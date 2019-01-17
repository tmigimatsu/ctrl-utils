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

namespace ctrl_utils {

class RedisClient : public cpp_redis::client {

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

  template<class... Args>
  std::future<cpp_redis::reply> mset(const Args&... args);

  template<class... Args>
  cpp_redis::reply sync_mset(const Args&... args);

  template<class... Ts>
  std::future<std::tuple<Ts...>> mget(const std::vector<std::string>& keys);

  template<class... Ts>
  std::tuple<Ts...> sync_mget(const std::vector<std::string>& keys);

  template<typename T>
  RedisClient& publish(const std::string& key, const T& value,
                       const reply_callback_t& reply_callback);

  template<typename T>
  std::future<cpp_redis::reply> publish(const std::string& key, const T& value);

  template<typename T>
  cpp_redis::reply sync_publish(const std::string& key, const T& value);

};


template<typename T>
RedisClient& RedisClient::set(const std::string& key, const T& value,
                              const reply_callback_t& reply_callback) {
  std::stringstream ss;
  ss << value;
  cpp_redis::client::set(key, ss.str(), reply_callback);
  return *this;
}

template<typename T>
std::future<cpp_redis::reply> RedisClient::set(const std::string& key, const T& value) {
  std::stringstream ss;
  ss << value;
  return cpp_redis::client::set(key, ss.str());
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
  cpp_redis::client::get(key, [key, reply_callback, error_callback](cpp_redis::reply& reply) {
    if (!reply.is_string()) {
      if (error_callback) {
        error_callback("RedisClient::get(): Failed to get string value from key: " + key + ".");
      }
      return;
    }
    std::stringstream ss(reply.as_string());
    T value;
    try {
      ss >> value;
    } catch (const std::exception& e) {
      if (error_callback) {
        error_callback("RedisClient::get(): Exception thrown on key: " + key + "\n\t" + e.what());
      }
      return;
    }
    reply_callback(std::move(value));
  });
  return *this;
}

template<>
inline RedisClient& RedisClient::get(const std::string& key,
    const std::function<void(std::string&&)>& reply_callback,
    const std::function<void(const std::string&)>& error_callback) {
  cpp_redis::client::get(key, [key, reply_callback, error_callback](cpp_redis::reply& reply) {
    if (!reply.is_string()) {
      if (error_callback) {
        error_callback("RedisClient::get(): Failed to get string value from key: " + key + ".");
      }
      return;
    }
    std::string str = reply.as_string();
    reply_callback(std::move(str));
  });
  return *this;
}

template<typename T>
std::future<T> RedisClient::get(const std::string& key) {
  auto promise = std::make_shared<std::promise<T>>();
  cpp_redis::client::get(key, [this, key, promise](cpp_redis::reply& reply) {
    if (!reply.is_string()) {
      std::string error("Redis::get(): Failed to get string value from key: " + key + ".");
      promise->set_exception(std::make_exception_ptr(std::runtime_error(error)));
      return;
    }
    std::stringstream ss(reply.as_string());
    T value;
    try {
      ss >> value;
    } catch (const std::exception& e) {
      std::string error("RedisClient::get(): Exception thrown on key: " + key + "\n\t" + e.what());
      promise->set_exception(std::make_exception_ptr(std::runtime_error(error)));
      return;
    }
    promise->set_value(std::move(value));
  });
  return promise->get_future();
}

template<>
inline std::future<std::string> RedisClient::get(const std::string& key) {
  auto promise = std::make_shared<std::promise<std::string>>();
  cpp_redis::client::get(key, [this, key, promise](cpp_redis::reply& reply) {
    if (!reply.is_string()) {
      std::string error("Redis::get(): Failed to get string value from key: " + key + ".");
      promise->set_exception(std::make_exception_ptr(std::runtime_error(error)));
      return;
    }
    promise->set_value(reply.as_string());
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
bool ReplyToString(const cpp_redis::reply& reply, T& value) {
  std::stringstream ss(reply.as_string());
  ss >> value;
  return true;
}

template<typename Tuple, size_t... Is>
void RepliesToTuple(const std::vector<cpp_redis::reply>& replies, Tuple& values,
                   std::index_sequence<Is...>) {
  std::initializer_list<bool>{ ReplyToString(replies[Is], std::get<Is>(values))... };
}

template<typename T>
bool KeyvalToString(const std::pair<std::string, T>& key_val,
                    std::pair<std::string, std::string>& key_valstr) {
  key_valstr.first = key_val.first;
  std::stringstream ss;
  ss << key_val.second;
  key_valstr.second = ss.str();
  return true;
}

template<typename Tuple, size_t... Is>
void KeyvalsToString(const Tuple& args,
                     std::vector<std::pair<std::string, std::string>>& key_valstr,
                     std::index_sequence<Is...>) {
  std::initializer_list<bool>{ KeyvalToString(std::get<Is>(args), key_valstr[Is])... };
}

template<class... Args>
std::future<cpp_redis::reply> RedisClient::mset(const Args&... args) {
  constexpr size_t num_pairs = sizeof...(Args);
  std::vector<std::pair<std::string, std::string>> key_valstr(num_pairs);
  KeyvalsToString(std::make_tuple(args...), key_valstr, std::index_sequence_for<Args...>{});
  return cpp_redis::client::mset(key_valstr);
}

template<class... Args>
cpp_redis::reply RedisClient::sync_mset(const Args&... args) {
  std::future<cpp_redis::reply> future = mset(args...);
  commit();
  return future.get();
}

template<class... Ts>
RedisClient& mget(const std::vector<std::string>& keys,
                  const std::function<void(Ts...)> reply_callback,
                  const std::function<void(const std::string&)>& error_callback = {}) {

}

template<class... Ts>
std::future<std::tuple<Ts...>> RedisClient::mget(const std::vector<std::string>& keys) {
  auto promise = std::make_shared<std::promise<std::tuple<Ts...>>>();
  cpp_redis::client::mget(keys, [this, keys, promise](cpp_redis::reply& reply) {
    if (!reply.is_array()) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (const std::string& key : keys) ss_error << " " << key;
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    std::tuple<Ts...> values;
    try {
      RepliesToTuple(reply.as_array(), values, std::index_sequence_for<Ts...>{});
    } catch (const std::exception& e) {
      std::stringstream ss_error("RedisClient::mget(): Failed to get values from keys:");
      for (const std::string& key : keys) ss_error << " " << key;
      ss_error << ".";
      promise->set_exception(std::make_exception_ptr(std::runtime_error(ss_error.str())));
      return;
    }
    promise->set_value(std::move(values));
  });
  return promise->get_future();
}

template<class... Ts>
std::tuple<Ts...> RedisClient::sync_mget(const std::vector<std::string>& keys) {
  std::future<std::tuple<Ts...>> future = mget<Ts...>(keys);
  commit();
  return future.get();
}

template<typename T>
RedisClient& RedisClient::publish(const std::string& key, const T& value,
                                  const reply_callback_t& reply_callback) {
  std::stringstream ss;
  ss << value;
  cpp_redis::client::publish(key, ss.str(), reply_callback);
  return *this;
}

template<typename T>
std::future<cpp_redis::reply> RedisClient::publish(const std::string& key, const T& value) {
  std::stringstream ss;
  ss << value;
  return cpp_redis::client::publish(key, ss.str());
}

template<typename T>
cpp_redis::reply RedisClient::sync_publish(const std::string& key, const T& value) {
  std::future<cpp_redis::reply> future = publish(key, value);
  commit();
  return future.get();
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_REDIS_CLIENT_H_
