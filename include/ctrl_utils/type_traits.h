/**
 * type_traits.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: May 10, 2019
 * Authors: Toki Migimatsu
 */

#include <type_traits>

#if __cplusplus < 201703L

#ifndef CTRL_UTILS_TYPE_TRAITS_H_
#define CTRL_UTILS_TYPE_TRAITS_H_

namespace std {

template<class...> struct conjunction : std::true_type { };
template<class B1> struct conjunction<B1> : B1 { };
template<class B1, class... Bn>
struct conjunction<B1, Bn...> 
    : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

template<class... B>
constexpr bool conjunction_v = conjunction<B...>::value;

}  // namespace std

#endif  // CTRL_UTILS_TYPE_TRAITS_H_

#endif  // __cplusplus
