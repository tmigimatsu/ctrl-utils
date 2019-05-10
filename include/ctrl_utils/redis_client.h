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

  template<typename T>
  RedisClient& set(const std::string& key, const T& value,
                   const reply_callback_t& reply_callback);

  template<typename T>
  std::future<cpp_redis::reply> set(const std::string& key, const T& value);

  template<typename T>
  cpp_redis::reply sync_set(const std::string& key, const T& value);

  template<typename T>
  RedisClient& get(const std::string& key,
                   const std::function<void(T&&)>& reply_callback,
                   const std::function<void(const std::string&)>& error_callback = {});

  template<typename T>
  std::future<T> get(const std::string& key);

  template<typename T>
  T sync_get(const std::string& key);

  /**
   * mset({"key1", val1}, {"key1", val2});
   * or
   * mset({{"key1", "strval1"}, {"key2", "strval2"}})
   */
  // template<class... Args>
  // std::future<cpp_redis::reply> mset(const Args&... args);

  // template<class... Args>
  // cpp_redis::reply sync_mset(const Args&... args);

  template<typename T>
  RedisClient& mset(const std::vector<std::pair<std::string, T>>& key_vals,
                    const reply_callback_t& reply_callback);

  template<typename T>
  std::future<cpp_redis::reply> mset(const std::vector<std::pair<std::string, T>>& key_vals);

  template<typename T>
  cpp_redis::reply sync_mset(const std::vector<std::pair<std::string, T>>& key_vals);

  /**
   * mget<double, int>("key1", "key2");
   */
  // template<class... Ts>
  // RedisClient& mget(const std::array<std::string, sizeof...(Ts)>& keys,
  //                   const std::function<void(std::tuple<Ts...>&&)> reply_callback,
  //                   const std::function<void(const std::string&)>& error_callback = {});

  template<class... Ts, class... Args, typename = is_all_strings<Args...>>
  std::future<std::tuple<Ts...>> mget(const Args&... args);

  template<class... Ts, class... Args, typename = is_all_strings<Args...>>
  std::tuple<Ts...> sync_mget(const Args&... args);

  template<typename T>
  std::future<std::vector<T>> mget(const std::vector<std::string>& keys);

  template<typename T>
  std::vector<T> sync_mget(const std::vector<std::string>& keys);

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

template<typename Func, typename Tuple, size_t... Is>
void Apply(const Func& func, Tuple&& values, std::index_sequence<Is...>) {
  func(std::move(std::get<Is>(values))...);
}

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

template<class... Ts, class... Args, typename>
std::tuple<Ts...> RedisClient::sync_mget(const Args&... args) {
  std::future<std::tuple<Ts...>> future = mget<Ts...>(args...);
  commit();
  return future.get();
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

template<typename T>
std::vector<T> RedisClient::sync_mget(const std::vector<std::string>& keys) {
  std::future<std::vector<T>> future = mget<T>(keys);
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
