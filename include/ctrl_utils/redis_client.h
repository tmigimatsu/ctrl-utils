/**
 * redis_client.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: July 1, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_REDIS_CLIENT_H_
#define CTRL_UTILS_REDIS_CLIENT_H_

#include <exception>   // std::exception
#include <future>      // std::future, std::promise
#include <functional>  // std::function
#include <string>      // std::string
#include <sstream>     // std::stringstream
#include <tuple>       // std::tuple, std::get
#include <utility>     // std::integer_sequence

#include <cpp_redis/cpp_redis>

#include "ctrl_utils/string.h"
#include "ctrl_utils/type_traits.h"

namespace ctrl_utils {

class RedisClient : public cpp_redis::client {

 private:

  template<typename... Ts>
  using is_all_strings = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Ts, std::string>...>>;

 public:

  RedisClient() : cpp_redis::client() {}

  /**
   * Asynchronous Redis GET command with std::future.
   *
   * Values will get converted from strings with ctrl_utils::FromString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator>>().
   * These specializations can be defined locally for custom types in your code.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * std::future<double> value = redis_client.get<double>("key");
   * redis_client.commit();
   * std::cout << "Value: " << value.get() << std::endl;
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @return Future Redis value.
   */
  template<typename T>
  std::future<T> get(const std::string& key);

  /**
   * Asynchronous Redis GET command with callbacks.
   *
   * Values will get converted from strings with ctrl_utils::FromString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator>>().
   * These specializations can be defined locally for custom types in your code.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * redis_client.get<double>("key", [](double&& value) {
   *   std::cout << "Value: " << value << std::endl;
   * }, [](const std::string& error) {
   *   std::cerr << "GET failed: " << error << std::endl;
   * });
   * redis_client.commit();
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @param reply_callback Callback function that gets the value result passed
   *                       in as an Rvalue reference the command has finished.
   * @param error_callback Callback function that gets called with the error
   *                       string when the command has failed.
   * @return RedisClient reference for command chaining.
   */
  template<typename T>
  RedisClient& get(const std::string& key,
                   const std::function<void(T&&)>& reply_callback,
                   const std::function<void(const std::string&)>& error_callback = {});

  /**
   * Synchronous Redis GET command with std::future.
   *
   * Values will get converted from strings with ctrl_utils::FromString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator>>().
   * These specializations can be defined locally for custom types in your code.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * double value = redis_client.sync_get<double>("key");
   * std::cout << "Value: " << value.get() << std::endl;
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @return Future Redis value.
   */
  template<typename T>
  T sync_get(const std::string& key);

  /**
   * Asynchronous Redis SET command with std::future.
   *
   * Values will get converted to strings with ctrl_utils::ToString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator<<().
   * These specializations can be defined locally for custom types in your code.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * redis_client.set("key1", 1234);
   * redis_client.set("key2", 1234);
   * redis_client.commit();
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @param value Redis value.
   * @return Future Redis reply.
   */
  template<typename T>
  std::future<cpp_redis::reply> set(const std::string& key, const T& value);

  /**
   * Asynchronous Redis SET command with callbacks.
   *
   * Values will get converted to strings with ctrl_utils::ToString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator<<().
   * These specializations can be defined locally for custom types in your code.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * redis_client.set("key", 1234, [](cpp_redis::reply& reply) {
   *   if (!reply) throw std::runtime_error("Set failed.");
   * });
   * redis_client.commit();
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @param value Redis value.
   * @param reply_callback Callback function of type
   *                       std::function<void(cpp_redis::reply&)> that gets
   *                       called when the command has finished.
   * @return RedisClient reference for command chaining.
   */
  template<typename T>
  RedisClient& set(const std::string& key, const T& value,
                   const reply_callback_t& reply_callback);

  /**
   * Synchronous Redis SET command.
   *
   * Values will get converted to strings with ctrl_utils::ToString<T>(), or if
   * a specialization for that type doesn't exist, std::stringstream::operator<<().
   * These specializations can be defined locally for custom types in your code.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * redis_client.sync_set("key", 1234);
   * ~~~~~~~~~~
   *
   * @param key Redis key.
   * @param value Redis value.
   * @return Redis reply.
   */
  template<typename T>
  cpp_redis::reply sync_set(const std::string& key, const T& value);

  /**
   * Asynchronous Redis MGET command with std::future.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * std::future<std::tuple<double, int>> fut_values = redis_client.mget<double, int>("key1", "key2");
   * redis_client.commit();
   * std::tuple<double, int> values = fut_values.get();
   * std::cout << std::get<0>(values) << " " << std::get<1>(values) << std::endl;
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Future Redis values packed into a std::tuple.
   */
  template<class... Ts, class... Strings, typename = is_all_strings<Strings...>>
  std::future<std::tuple<Ts...>> mget(const Strings&... keys);

  /**
   * Asynchronous Redis MGET command with std::future for a vector of a single type.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * std::future<std::vector<int>> fut_values = redis_client.mget<int>({"key1", "key2"});
   * redis_client.commit();
   * for (int value : fut_values.get()) {
   *   std::cout << value << std::endl;
   * }
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Future Redis values packed into a std::vector.
   */
  template<typename T>
  std::future<std::vector<T>> mget(const std::vector<std::string>& keys);

  /**
   * Synchronous Redis MGET command.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * std::tuple<double, int> values = redis_client.sync_mget<double, int>("key1", "key2");
   * std::cout << std::get<0>(values) << " " << std::get<1>(values) << std::endl;
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Redis values in a std::tuple.
   */
  template<class... Ts, class... Strings, typename = is_all_strings<Strings...>>
  std::tuple<Ts...> sync_mget(const Strings&... keys);

  /**
   * Synchronous Redis MGET command for a vector of a single type.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * std::vector<int> values = redis_client.sync_mget<int>({"key1", "key2"});
   * for (int value : fut_values.get()) {
   *   std::cout << value << std::endl;
   * }
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Redis values in a std::vector.
   */
  template<typename T>
  std::vector<T> sync_mget(const std::vector<std::string>& keys);

  // template<class... Ts>
  // RedisClient& mget(const std::array<std::string, sizeof...(Ts)>& keys,
  //                   const std::function<void(std::tuple<Ts...>&&)> reply_callback,
  //                   const std::function<void(const std::string&)>& error_callback = {});

  /**
   * Asynchronous Redis MSET command with std::future.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * // key1 and key2 will be set atomically.
   * std::vector<std::pair<std::string, int>> key_vals;
   * key_vals.emplace_back("key1", 1234);
   * key_vals.emplace_back("key2", 1234);
   * redis_client.mset(key_vals);
   *
   * // Brace-initialization of key-value pairs requires explicit template specification.
   * // key3 and key4 will not necessarily be set atomically with key1 and key2.
   * redis_client.mset<int>({{"key3", 1234}, {"key4", 1234}});
   *
   * redis_client.commit();
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Future Redis reply.
   */
  template<typename T>
  std::future<cpp_redis::reply> mset(const std::vector<std::pair<std::string, T>>& key_vals);

  /**
   * Asynchronous Redis MSET command with callbacks.
   *
   * Commands are not sent until RedisClient::commit() is called.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * // key1 and key2 will be set atomically.
   * std::vector<std::pair<std::string, int>> key_vals;
   * key_vals.emplace_back("key1", 1234);
   * key_vals.emplace_back("key2", 1234);
   * redis_client.mset(key_vals, [](cpp_redis::reply& reply) {
   *   if (!reply) throw std::runtime_error("Set failed.");
   * });
   *
   * // Brace-initialization of key-value pairs requires explicit template specification.
   * // key3 and key4 will not necessarily be set atomically with key1 and key2.
   * redis_client.mset<int>({{"key3", 1234}, {"key4", 1234}});
   *
   * redis_client.commit();
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @param reply_callback Callback function of type
   *                       std::function<void(cpp_redis::reply&)> that gets
   *                       called when the command has finished.
   * @return RedisClient reference for command chaining.
   */
  template<typename T>
  RedisClient& mset(const std::vector<std::pair<std::string, T>>& key_vals,
                    const reply_callback_t& reply_callback);

  /**
   * Synchronous Redis MSET command.
   *
   * Example:
   * ~~~~~~~~~~ {.c}
   * // key1 and key2 will be set atomically.
   * std::vector<std::pair<std::string, int>> key_vals;
   * key_vals.emplace_back("key1", 1234);
   * key_vals.emplace_back("key2", 1234);
   * redis_client.sync_mset(key_vals);
   *
   * // Brace-initialization of key-value pairs requires explicit template specification.
   * // key3 and key4 will not necessarily be set atomically with key1 and key2.
   * redis_client.sync_mset<int>({{"key3", 1234}, {"key4", 1234}});
   * ~~~~~~~~~~
   *
   * @param key_vals Vector of key-value pairs.
   * @return Redis reply.
   */
  template<typename T>
  cpp_redis::reply sync_mset(const std::vector<std::pair<std::string, T>>& key_vals);

  // template<class... Args>
  // std::future<cpp_redis::reply> mset(const Args&... args);

  // template<class... Args>
  // cpp_redis::reply sync_mset(const Args&... args);

  template<typename T>
  RedisClient& hset(const std::string& key, const std::string& field, const T& value,
                    const reply_callback_t& reply_callback);

  template<typename T>
  std::future<cpp_redis::reply> hset(const std::string& key, const std::string& field, const T& value);

  template<typename T>
  cpp_redis::reply sync_hset(const std::string& key, const std::string& field, const T& value);

  template<typename T>
  RedisClient& publish(const std::string& key, const T& value,
                       const reply_callback_t& reply_callback);

  template<typename T>
  std::future<cpp_redis::reply> publish(const std::string& key, const T& value);

  template<typename T>
  cpp_redis::reply sync_publish(const std::string& key, const T& value);

 private:

  template<typename T>
  static bool ReplyToString(const cpp_redis::reply& reply, T& value);

  template<typename Tuple, size_t... Is>
  static void RepliesToTuple(const std::vector<cpp_redis::reply>& replies, Tuple& values,
                             std::index_sequence<Is...>);

};


/**
 * Implementation
 */

template<typename T>
RedisClient& RedisClient::get(const std::string& key,
                              const std::function<void(T&&)>& reply_callback,
                              const std::function<void(const std::string&)>& error_callback) {
  send({"GET", key}, [key, reply_callback, error_callback](cpp_redis::reply& reply) {
    if (!reply.is_string()) {
      if (error_callback) {
        error_callback("RedisClient::get(): Failed to get string value from key: " + key + ".");
      }
      return;
    }
    try {
      reply_callback(FromString<T>(reply.as_string()));
    } catch (const std::exception& e) {
      if (error_callback) {
        error_callback("RedisClient::get(): Exception thrown on key: " + key + "\n\t" + e.what());
      }
      return;
    }
  });
  return *this;
}

template<typename T>
std::future<T> RedisClient::get(const std::string& key) {
  auto promise = std::make_shared<std::promise<T>>();
  get<T>(key, [promise](T&& value) {
    promise->set_value(std::move(value));
  }, [promise](const std::string& error) {
    promise->set_exception(std::make_exception_ptr(std::runtime_error(error)));
  });
  return promise->get_future();
}

template<typename T>
T RedisClient::sync_get(const std::string& key) {
  std::future<T> future = get<T>(key);
  commit();
  return future.get();
};

template<typename T>
RedisClient& RedisClient::set(const std::string& key, const T& value,
                              const reply_callback_t& reply_callback) {
  send({"SET", key, ToString(value)}, reply_callback);
  return *this;
}

template<typename T>
std::future<cpp_redis::reply> RedisClient::set(const std::string& key, const T& value) {
  auto promise = std::make_shared<std::promise<cpp_redis::reply>>();
  set(key, value, [promise](cpp_redis::reply& reply) {
    if (reply) {
      promise->set_value(reply);
    } else {
      promise->set_exception(std::make_exception_ptr(std::runtime_error(reply.error())));
    }
  });
  return promise->get_future();
}

template<typename T>
cpp_redis::reply RedisClient::sync_set(const std::string& key, const T& value) {
  std::future<cpp_redis::reply> future = set(key, value);
  commit();
  return future.get();
};

template<typename T>
bool RedisClient::ReplyToString(const cpp_redis::reply& reply, T& value) {
  FromString(reply.as_string(), value);
  return true;
}

template<typename Tuple, size_t... Is>
void RedisClient::RepliesToTuple(const std::vector<cpp_redis::reply>& replies, Tuple& values,
                                 std::index_sequence<Is...>) {
  std::initializer_list<bool>{ ReplyToString(replies[Is], std::get<Is>(values))... };
}

// template<typename T>
// bool KeyvalToString(const std::pair<std::string, T>& key_val,
//                     std::pair<std::string, std::string>& key_valstr) {
//   key_valstr.first = key_val.first;
//   ToString(key_valstr.second, key_val.second);
//   return true;
// }

// template<typename Tuple, size_t... Is>
// void KeyvalsToString(const Tuple& args,
//                      std::vector<std::pair<std::string, std::string>>& key_valstr,
//                      std::index_sequence<Is...>) {
//   std::initializer_list<bool>{ KeyvalToString(std::get<Is>(args), key_valstr[Is])... };
// }

// template<class... Args>
// std::future<cpp_redis::reply> RedisClient::mset(const Args&... args) {
//   constexpr size_t num_pairs = sizeof...(Args);
//   std::vector<std::pair<std::string, std::string>> key_valstr(num_pairs);
//   KeyvalsToString(std::make_tuple(args...), key_valstr, std::index_sequence_for<Args...>{});
//   return cpp_redis::client::mset(key_valstr);
// }

// template<class... Args>
// cpp_redis::reply RedisClient::sync_mset(const Args&... args) {
//   std::future<cpp_redis::reply> future = mset(args...);
//   commit();
//   return future.get();
// }

template<class... Ts, class... Args, typename>
std::future<std::tuple<Ts...>> RedisClient::mget(const Args&... args) {
  static_assert(sizeof...(Ts) == sizeof...(Args), "Number of keys must equal number of output types.");

  auto promise = std::make_shared<std::promise<std::tuple<Ts...>>>();
  // const std::array<std::string, sizeof...(Ts)> keys = {{ args... }};
  // mget<Ts...>(keys, [promise](Ts&&... values) {
  //   promise->set_value(std::tuple<Ts...>{ values... });
  // }, [promise](const std::string& error) {
  //   promise->set_exception(std::make_exception_ptr(std::runtime_error(error)));
  // });
  std::vector<std::string> command = {"MGET", args...};
  send(command, [this, command, promise](cpp_redis::reply& reply) {
    if (!reply.is_array()) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    std::tuple<Ts...> values;
    try {
      RepliesToTuple(reply.as_array(), values, std::index_sequence_for<Ts...>{});
    } catch (const std::exception& e) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    promise->set_value(std::move(values));
  });
  return promise->get_future();
}

template<typename T>
std::future<std::vector<T>> RedisClient::mget(const std::vector<std::string>& keys) {
  auto promise = std::make_shared<std::promise<std::vector<T>>>();
  std::vector<std::string> command(keys.size());
  command[0] = "MGET";
  std::copy(keys.begin(), keys.end(), command.begin() + 1);
  send(command, [this, command, promise](cpp_redis::reply& reply) {
    if (!reply.is_array()) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    std::vector<T> values;
    values.reserve(reply.as_array().size());
    try {
      for (const cpp_redis::reply& r : reply.as_array()) {
        values.push_back(FromString<T>(r.as_string()));
      }
    } catch (const std::exception& e) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    promise->set_value(std::move(values));
  });
  return promise->get_future();
}

template<class... Ts, class... Args, typename>
std::tuple<Ts...> RedisClient::sync_mget(const Args&... args) {
  std::future<std::tuple<Ts...>> future = mget<Ts...>(args...);
  commit();
  return future.get();
}

template<typename T>
std::vector<T> RedisClient::sync_mget(const std::vector<std::string>& keys) {
  std::future<std::vector<T>> future = mget<T>(keys);
  commit();
  return future.get();
}

// template<typename Func, typename Tuple, size_t... Is>
// void Apply(const Func& func, Tuple&& values, std::index_sequence<Is...>) {
//   func(std::move(std::get<Is>(values))...);
// }

// template<class... Ts>
// RedisClient& RedisClient::mget(const std::array<std::string, sizeof...(Ts)>& keys,
//                                const std::function<void(std::tuple<Ts...>&&)> reply_callback,
//                                const std::function<void(const std::string&)>& error_callback) {

//   std::vector<std::string> command = {"MGET", args...};
//   send(command, [command, reply_callback, error_callback](cpp_redis::reply& reply) {
//     if (!reply.is_array()) {
//       std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
//       for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
//       ss_error << ".";
//       error_callback(ss_error.str());
//       return;
//     }
//     std::tuple<Ts...> values;
//     try {
//       RepliesToTuple(reply.as_array(), values, std::index_sequence_for<Ts...>{});
//     } catch (const std::exception& e) {
//       std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
//       for (size_t i = 1; i < command.size(); i++) ss_error << " " << command[i];
//       ss_error << ".";
//       error_callback(ss_error.str());
//       return;
//     }
//     // Apply(reply_callback, values, std::index_sequence_for<Ts...>{});
//   });
//   return *this;
// }

template<typename T>
RedisClient& RedisClient::mset(const std::vector<std::pair<std::string, T>>& key_vals,
                               const reply_callback_t& reply_callback) {
  std::vector<std::string> command;
  command.reserve(2 * key_vals.size() + 1);
  command[0] = "MSET";
  for (const std::pair<std::string, T>& key_val : key_vals) {
    command.push_back(key_val.first);
    command.push_back(ToString(key_val.second));
  }
  send(command, reply_callback);
  return *this;
}

template<typename T>
std::future<cpp_redis::reply> RedisClient::mset(const std::vector<std::pair<std::string, T>>& key_vals) {
  auto promise = std::make_shared<std::promise<cpp_redis::reply>>();
  mset(key_vals, [promise](cpp_redis::reply& reply) {
    if (reply) {
      promise->set_value(reply);
    } else {
      promise->set_exception(std::make_exception_ptr(std::runtime_error(reply.error())));
    }
  });
  return promise->get_future();
}

template<typename T>
cpp_redis::reply RedisClient::sync_mset(const std::vector<std::pair<std::string, T>>& key_vals) {
  std::future<cpp_redis::reply> future = mset(key_vals);
  commit();
  return future.get();
}

template<typename T>
RedisClient& RedisClient::publish(const std::string& key, const T& value,
                                  const reply_callback_t& reply_callback) {
  std::string str;
  ToString(str, value);
  cpp_redis::client::publish(key, str, reply_callback);
  return *this;
}

template<typename T>
RedisClient& RedisClient::hset(const std::string& key, const std::string& field,
                               const T& value, const reply_callback_t& reply_callback) {
  std::string str;
  ToString(str, value);
  cpp_redis::client::hset(key, field, str, reply_callback);
  return *this;
}

template<typename T>
std::future<cpp_redis::reply> RedisClient::hset(const std::string& key, const std::string& field,
                                                const T& value) {
  std::string str;
  ToString(str, value);
  return cpp_redis::client::hset(key, field, str);
}

template<typename T>
cpp_redis::reply RedisClient::sync_hset(const std::string& key, const std::string& field,
                                        const T& value) {
  std::future<cpp_redis::reply> future = hset(key, field, value);
  commit();
  return future.get();
};

template<typename T>
std::future<cpp_redis::reply> RedisClient::publish(const std::string& key, const T& value) {
  std::string str;
  ToString(str, value);
  return cpp_redis::client::publish(key, str);
}

template<typename T>
cpp_redis::reply RedisClient::sync_publish(const std::string& key, const T& value) {
  std::future<cpp_redis::reply> future = publish(key, value);
  commit();
  return future.get();
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_REDIS_CLIENT_H_
