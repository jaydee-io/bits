////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_UNDERLYING_INTEGRAL_TYPE_H
#define BITS_UNDERLYING_INTEGRAL_TYPE_H

#include <cstdint>
#include <type_traits>

namespace bits::detail {

template<size_t N, bool isSigned>
struct underlying_integral_type : std::false_type {};

template<> struct underlying_integral_type<0, false> { using type = uint8_t; };
template<> struct underlying_integral_type<1, false> { using type = uint8_t; };
template<> struct underlying_integral_type<2, false> { using type = uint16_t; };
template<> struct underlying_integral_type<3, false> { using type = uint32_t; };
template<> struct underlying_integral_type<4, false> { using type = uint32_t; };
template<> struct underlying_integral_type<5, false> { using type = uint64_t; };
template<> struct underlying_integral_type<6, false> { using type = uint64_t; };
template<> struct underlying_integral_type<7, false> { using type = uint64_t; };
template<> struct underlying_integral_type<8, false> { using type = uint64_t; };

template<> struct underlying_integral_type<0, true> { using type = int8_t; };
template<> struct underlying_integral_type<1, true> { using type = int8_t; };
template<> struct underlying_integral_type<2, true> { using type = int16_t; };
template<> struct underlying_integral_type<3, true> { using type = int32_t; };
template<> struct underlying_integral_type<4, true> { using type = int32_t; };
template<> struct underlying_integral_type<5, true> { using type = int64_t; };
template<> struct underlying_integral_type<6, true> { using type = int64_t; };
template<> struct underlying_integral_type<7, true> { using type = int64_t; };
template<> struct underlying_integral_type<8, true> { using type = int64_t; };

template<typename T>
using underlying_integral_type_t = typename underlying_integral_type<sizeof(T), std::is_signed_v<T>>::type;

} // namespace bits::detail

#endif /* BITS_UNDERLYING_INTEGRAL_TYPE_H */