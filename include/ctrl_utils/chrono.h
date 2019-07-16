/**
 * chrono.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: July 16, 2019
 * Authors: Toki Migimatsu
 */

#if __cplusplus < 201703L

#include <chrono>       // std::chrono
#include <type_traits>  // std::enable_if_t

namespace std {
namespace chrono {

template<class T> struct is_duration : std::false_type {};
template<class Rep, class Period> struct is_duration<std::chrono::duration<Rep, Period>>
    : std::true_type {};

template<class To, class Rep, class Period, class = std::enable_if_t<is_duration<To>{}>>
constexpr To floor(const duration<Rep, Period>& d) {
    To t = std::chrono::duration_cast<To>(d);
    if (t > d) return t - To{1};
    return t;
}

template<class To, class Clock, class FromDuration, class = std::enable_if_t<is_duration<To>{}>>
constexpr std::chrono::time_point<Clock, To>
floor(const std::chrono::time_point<Clock, FromDuration>& tp) {
    return std::chrono::time_point<Clock, To>{ std::chrono::floor<To>(tp.time_since_epoch()) };
}

}  // namespace chrono
}  // namespace std

#endif  // __cplusplus
