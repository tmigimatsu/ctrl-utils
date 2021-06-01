/**
 * argparse.h
 *
 * Copyright 2021. All Rights Reserved.
 *
 * Created: May 27, 2021
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_ARGPARSE_H_
#define CTRL_UTILS_ARGPARSE_H_

#include <ctrl_utils/string.h>

#include <algorithm>      // std::find
#include <exception>      // std::invalid_argument
#include <iomanip>        // std::setw, std::left
#include <iostream>       // std::cout
#include <list>           // std::list
#include <memory>         // std::shared_ptr
#include <optional>       // std::optional
#include <sstream>        // std::stringstream
#include <string>         // std::string
#include <string_view>    // std::string_view
#include <unordered_map>  // std::unordered_map
#include <utility>        // std::move, std::pair
#include <vector>         // std::vector

namespace ctrl_utils {

/**
 * Parses the arguments.
 *
 * Example:
 * @code
 * struct Args : ctrl_utils::Args {
 *   // Mandatory constructor boilerplate.
 *   explicit Args(ctrl_utils::Args&& args)
 *     : ctrl_utils::Args(std::move(args)) {}
 *
 *   // Create a positional string argument.
 *   // E.g. `./hello joe`
 *   std::string name = Arg<std::string>("name", "Your name.");
 *
 *   // Create an optional int argument.
 *   // E.g. `./hello joe --num-repeat 5` or `./hello joe -n 5`.
 *   int num_repeat = Kwarg<int>("n,num-repeat", 1,
 *       "Number of times to repeat the greeting");
 *
 *   // Create a flag argument.
 *   // E.g. `./bin joe -n 5 --print-name` or `./bin joe -n 5 --no-print-name`.
 *   bool print_name = Flag<bool>("print-name", true, "Print your name.");
 * };
 *
 * int main(int argc, char* argv[]) {
 *   // Parse the arguments into an Args object.
 *   std::optional<Args> args = ctrl_utils::ParseArgs<Args>(argc, argv);
 *
 *   // If the optional is empty, the args could not be parsed.
 *   if (!args) return 1;
 *
 *   // Access the parsed args as normal fields in the struct.
 *   const std::string entity = args->print_name ? args->name : "world";
 *   for (int i = 0; i < args->num_repeat; i++) {
 *     std::cout << "Hello " << entity << "!" << std::endl;
 *   }
 *   return 0;
 * }
 * @endcode
 *
 * @returns A child class of ctrl_utils::Args with its fields populated with the
 *          command line arguments, or an empty optional if the parsing fails.
 */
template <typename Derived>
std::optional<Derived> ParseArgs(int argc, char* argv[]);

/**
 * Base Args class.
 *
 * Subclass this class and use the `Arg()`, `Kwarg()`, and `Flag()` methods to
 * declare and initialize the fields of the subclass.
 */
class Args {
 public:
  /**
   * Help string generated from the defined arguments.
   */
  const std::string& help_string() const { return help_string_; }

  /**
   * Prints the parsed fields of the Args object.
   */
  friend std::ostream& operator<<(std::ostream& os, const Args& args);

 protected:
  /**
   * Declares a positional argument.
   *
   * Prints an error if the argument cannot be parsed.
   *
   * @param name Name of the argument (only used to generate the help string).
   * @param description Description of the argument.
   * @returns The parsed positional argument.
   */
  template <typename T>
  T Arg(std::string_view name, std::string_view description) {
    return InitializeOrParseArg<PositionalParam, T>(true, name, {},
                                                    description);
  }

  /**
   * Declares a keyword argument.
   *
   * Single-character keywords use the form `-k`, and multi-character keywords
   * use the form `--keyword`. Prints an error if the argument cannot be parsed.
   *
   * @param keys Comma-separated string of keywords (e.g. "k,keyword").
   * @param default_value Default value of the argument.
   * @param description Description of the argument.
   * @returns The parsed keyword argument.
   */
  template <typename T>
  T Kwarg(std::string_view keys, T&& default_value,
          std::string_view description) {
    return InitializeOrParseArg<KeywordParam>(
        false, keys, std::forward<T>(default_value), description);
  }

  /**
   * Declares a flag argument.
   *
   * Positive flags use the form `--flag`, and negative flags use the form
   * `--no-flag`. Prints an error if the argument cannot be parsed.
   *
   * @param name Name of the flag (e.g. "flag").
   * @param default_value Default value of the flag.
   * @param description Description of the argument.
   * @returns The parsed flag argument.
   */
  bool Flag(std::string_view name, bool default_value,
            std::string_view description) {
    return InitializeOrParseArg<FlagParam>(false, name, default_value,
                                           description);
  }

 private:
  static constexpr std::string_view kTab = "    ";

  class Param {
   public:
    Param(std::string_view name, std::string_view description)
        : name(name), description(description) {}

    std::string_view name;
    std::string_view description;

   protected:
    static constexpr size_t kLenKey = 3 * Args::kTab.size();

    virtual void Print(std::ostream& os) const = 0;

    void PrintDescription(std::ostream& os, size_t len_keys) const;

    friend std::ostream& operator<<(std::ostream& os, const Param& param) {
      param.Print(os);
      return os;
    }
  };

  class PositionalParam : public Param {
   public:
    template <typename T>
    PositionalParam(std::string_view name, const T& default_value,
                    std::string_view description)
        : Param(name, description) {}

    template <typename T>
    void Parse(std::list<std::string_view>& args, T& val) const;

   protected:
    virtual void Print(std::ostream& os) const override;
  };

  class KeywordParam : public Param {
   public:
    template <typename T>
    KeywordParam(std::string_view keys, const T& default_value,
                 std::string_view description)
        : Param({}, description),
          keywords(ParseKeysAndName(keys, name)),
          default_value(Args::ParseValue(default_value)) {}

    std::vector<std::string> keywords;
    std::string default_value;

    template <typename T>
    void Parse(std::list<std::string_view>& args, T& val) const;

   protected:
    static std::vector<std::string> ParseKeysAndName(std::string_view keys,
                                                     std::string_view& name);

    virtual void Print(std::ostream& os) const override;
  };

  class FlagParam : public Param {
   public:
    FlagParam(std::string_view name, bool default_value,
              std::string_view description)
        : Param(name, description),
          positive_keyword("--" + std::string{name}),
          negative_keyword("--no-" + std::string{name}),
          default_keyword(default_value ? positive_keyword : negative_keyword) {
    }

    std::string positive_keyword;
    std::string negative_keyword;
    std::string_view default_keyword;

    template <typename T>
    void Parse(std::list<std::string_view>& args, T& val) const {}

   protected:
    virtual void Print(std::ostream& os) const override;
  };

  class Parser {
   public:
    Parser(int argc, char* argv[])
        : args_(TokenizeArgs(argc, argv)), name_app_(argv[0]) {}

    template <typename ParamT, typename T>
    T InitializeArg(bool is_required, std::string_view key, T&& default_value,
                    std::string_view description);

    template <typename ParamT, typename T>
    T ParseArg(
        std::string_view key, T&& val,
        std::vector<std::pair<std::string_view, std::string>>& parsed_args);

    static std::list<std::string_view> TokenizeArgs(int argc, char* argv[]);

    std::string GenerateHelpString() const;

    void CheckRemainingArguments() const;

   private:
    std::string_view name_app_;

    // List of tokens that gets cleared during parsing.
    std::list<std::string_view> args_;

    // Map from name to param for use during parsing.
    std::unordered_map<std::string, std::shared_ptr<Param>> params_;

    // List of positional params for generating the help string.
    std::vector<std::shared_ptr<Param>> required_params_;

    // List of optional params for generating the help string.
    std::vector<std::shared_ptr<Param>> optional_params_;
  };

  Args(int argc, char* argv[])
      : parser_(std::make_unique<Parser>(argc, argv)) {}

  template <typename ParamT, typename T>
  T InitializeOrParseArg(bool is_required, std::string_view key,
                         T&& default_value, std::string_view description);

  void SwitchToParsingStage();

  void Cleanup();

  template <typename T>
  static std::string ParseValue(const T& default_value);

  bool initializing_ = true;

  std::string help_string_;

  // List of keys and parsed values for printing the result.
  std::vector<std::pair<std::string_view, std::string>> parsed_args_;

  std::unique_ptr<Parser> parser_;

  template <typename Derived>
  friend std::optional<Derived> ParseArgs(int, char*[]);
};

////////////////////////
// Procedural methods //
////////////////////////

template <typename Derived>
std::optional<Derived> ParseArgs(int argc, char* argv[]) {
  // Initialize empty arg parser.
  Derived derived(Args(argc, argv));

  // Switch to parsing stage.
  derived.SwitchToParsingStage();

  // Parse args.
  Args& base_args = dynamic_cast<Args&>(derived);

  // Hold onto help string for exception catching.
  std::string_view help = base_args.help_string();
  try {
    Derived args(std::move(base_args));
    args.Cleanup();
    return std::move(args);
  } catch (std::invalid_argument& e) {
    std::cout << help << std::endl
              << bold << "Error: " << normal << e.what() << std::endl;
  }
  return {};
}

void Args::SwitchToParsingStage() {
  initializing_ = false;
  help_string_ = parser_->GenerateHelpString();
}

void Args::Cleanup() {
  parser_->CheckRemainingArguments();
  parser_.reset();
}

template <typename ParamT, typename T>
T Args::InitializeOrParseArg(bool is_required, std::string_view key,
                             T&& default_value, std::string_view description) {
  if (initializing_) {
    return parser_->InitializeArg<ParamT>(
        is_required, key, std::forward<T>(default_value), description);
  }
  return parser_->ParseArg<ParamT>(key, std::forward<T>(default_value),
                                   parsed_args_);
}

template <typename ParamT, typename T>
T Args::Parser::InitializeArg(bool is_required, std::string_view key,
                              T&& default_value, std::string_view description) {
  auto param = std::make_shared<ParamT>(key, default_value, description);
  params_.emplace(std::string{key}, param);

  std::vector<std::shared_ptr<Param>>& ordered_params =
      is_required ? required_params_ : optional_params_;
  ordered_params.push_back(std::move(param));
  return std::forward<T>(default_value);
}

template <typename ParamT, typename T>
T Args::Parser::ParseArg(
    std::string_view key, T&& val,
    std::vector<std::pair<std::string_view, std::string>>& parsed_args) {
  const auto* param =
      dynamic_cast<const ParamT*>(params_.at(std::string{key}).get());
  param->Parse(args_, val);

  parsed_args.emplace_back(param->name, Args::ParseValue(val));
  return std::forward<T>(val);
}

/////////////////////
// Parsing methods //
/////////////////////

template <typename T>
void Args::PositionalParam::Parse(std::list<std::string_view>& args,
                                  T& val) const {
  // Verify next arg matches this parameter.
  if (args.empty() || args.front()[0] == '-') {
    std::stringstream ss;
    ss << "Missing positional argument: " << bold << name << normal;
    throw std::invalid_argument(ss.str());
  }

  // Remove positional arg from args.
  try {
    val = ctrl_utils::FromString<T>(args.front());
  } catch (...) {
    std::stringstream ss;
    ss << "Unable to parse positional argument: " << bold << args.front()
       << normal;
    throw std::invalid_argument(ss.str());
  }
  args.pop_front();
}

template <typename T>
void Args::KeywordParam::Parse(std::list<std::string_view>& args,
                               T& val) const {
  for (auto it = args.begin(); it != args.end(); ++it) {
    const std::string_view& arg = *it;

    // Skip if arg not in keywords.
    if (std::find(keywords.begin(), keywords.end(), arg) == keywords.end())
      continue;

    // Parse value.
    auto it_val = it;
    ++it_val;
    if (it_val == args.end()) {
      std::stringstream ss;
      ss << "Missing value for keyword: " << bold << arg << normal;
      throw std::invalid_argument(ss.str());
    }
    try {
      val = ctrl_utils::FromString<T>(*it_val);
    } catch (...) {
      std::stringstream ss;
      ss << "Unable to parse value for keyword argument: " << bold << arg << " "
         << *it_val << normal;
      throw std::invalid_argument(ss.str());
    }

    // Remove keyword and value from args.
    ++it_val;
    args.erase(it, it_val);
    break;
  }
}

template <>
void Args::FlagParam::Parse(std::list<std::string_view>& args,
                            bool& val) const {
  for (auto it = args.begin(); it != args.end(); ++it) {
    const std::string_view& arg = *it;

    // Check if arg is positive, negative, or neither.
    if (arg == positive_keyword) {
      val = true;
    } else if (arg == negative_keyword) {
      val = false;
    } else {
      continue;
    }

    // Remove flag from args.
    args.erase(it);
    break;
  }
}

void Args::Parser::CheckRemainingArguments() const {
  if (!args_.empty()) {
    std::stringstream ss;
    ss << "Unrecognized arguments:" << bold;
    for (auto it = args_.begin(); it != args_.end(); ++it) {
      ss << " " << *it;
    }
    ss << normal;
    throw std::invalid_argument(ss.str());
  }
}

//////////////////////
// Printing methods //
//////////////////////

void Args::PositionalParam::Print(std::ostream& os) const {
  // Print name.
  os << Args::kTab << bold << name << normal;

  // Print description.
  Param::PrintDescription(os, name.size());
}

void Args::KeywordParam::Print(std::ostream& os) const {
  size_t len_keys = 0;

  // Print keywords.
  os << Args::kTab;
  std::string_view delimiter;
  for (const std::string& keyword : keywords) {
    os << delimiter << bold << keyword << normal;
    len_keys += delimiter.size() + keyword.size();
    if (delimiter.empty()) delimiter = ", ";
  }

  // Print name.
  os << " " << underline << name << normal;
  len_keys += 1 + name.size();

  // Print description.
  Param::PrintDescription(os, len_keys);
  os << " [default " << bold << default_value << normal << "]";
}

void Args::FlagParam::Print(std::ostream& os) const {
  // Print keywords.
  os << Args::kTab << bold << positive_keyword << normal << "/" << bold
     << negative_keyword << normal;

  // Print description.
  const size_t len_keys = positive_keyword.size() + 1 + negative_keyword.size();
  Param::PrintDescription(os, len_keys);
  os << " [default " << bold << default_keyword << normal << "]";
}

void Args::Param::PrintDescription(std::ostream& os, size_t len_keys) const {
  // Print spacing.
  if (len_keys < kLenKey) {
    os << std::setw(kLenKey - len_keys) << " ";
  } else {
    os << std::endl << Args::kTab << std::setw(kLenKey) << " ";
  }

  // Print description.
  os << description;
}

std::string Args::Parser::GenerateHelpString() const {
  std::stringstream ss;
  ss << "Usage:" << std::endl << Args::kTab << bold << name_app_ << normal;
  for (const std::shared_ptr<Param>& param : required_params_) {
    ss << " " << param->name;
  }
  ss << " [options...]" << std::endl;
  if (!required_params_.empty()) {
    ss << std::endl << "Required arguments:" << std::endl;
    for (const std::shared_ptr<Param>& param : required_params_) {
      ss << *param << std::endl;
    }
  }
  if (!optional_params_.empty()) {
    ss << std::endl << "Optional arguments:" << std::endl;
    for (const std::shared_ptr<Param>& param : optional_params_) {
      ss << *param << std::endl;
    }
  }
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Args& args) {
  // Get max key length;
  size_t max_len_key = 0;
  for (const std::pair<std::string_view, std::string>& key_val :
       args.parsed_args_) {
    if (key_val.first.size() > max_len_key) {
      max_len_key = key_val.first.size();
    }
  }

  os << "Parsed args:" << std::endl;
  for (const std::pair<std::string_view, std::string>& key_val :
       args.parsed_args_) {
    os << Args::kTab << bold << std::setw(max_len_key + 2) << std::left
       << key_val.first << normal << key_val.second << std::endl;
  }
  return os;
}

/////////////////////
// Utility methods //
/////////////////////

template <typename T>
std::string Args::ParseValue(const T& default_value) {
  std::string str_default = ToString(default_value);
  if (str_default.empty()) str_default = "\'\'";
  return str_default;
}

std::vector<std::string> Args::KeywordParam::ParseKeysAndName(
    std::string_view keys, std::string_view& name) {
  std::vector<std::string> parsed_keys;
  std::string_view longest_key;

  // Tokenize keys by ','.
  for (size_t idx_start = 0; idx_start < keys.size(); idx_start++) {
    for (size_t idx_end = idx_start; idx_end <= keys.size(); idx_end++) {
      if (idx_end != keys.size() && keys[idx_end] != ',') continue;

      // Get key.
      const std::string_view key = keys.substr(idx_start, idx_end - idx_start);

      // Check if key is longest one.
      if (key.size() > longest_key.size()) {
        longest_key = key;
      }

      // Prepend dashes to key.
      const char* dashes = key.size() == 1 ? "-" : "--";
      parsed_keys.push_back(dashes + std::string{key});

      // Advance idx_start to comma.
      idx_start = idx_end;
      break;
    }
  }

  // Set name to longest key.
  name = longest_key;

  return parsed_keys;
}

std::list<std::string_view> Args::Parser::TokenizeArgs(int argc, char* argv[]) {
  std::list<std::string_view> args;
  for (int i = 1; i < argc; i++) {
    args.emplace_back(argv[i]);
  }
  return args;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_ARGPARSE_H_
