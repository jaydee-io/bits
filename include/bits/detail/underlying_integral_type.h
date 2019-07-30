#ifndef BITS_UNDERLYING_INTEGRAL_TYPE_H
#define BITS_UNDERLYING_INTEGRAL_TYPE_H

#include <cstdint>

namespace bits::detail {

template<size_t N>
struct underlying_integral_type : std::false_type {};

template<> struct underlying_integral_type<0> { using type = uint8_t; };
template<> struct underlying_integral_type<1> { using type = uint8_t; };
template<> struct underlying_integral_type<2> { using type = uint16_t; };
template<> struct underlying_integral_type<3> { using type = uint32_t; };
template<> struct underlying_integral_type<4> { using type = uint32_t; };
template<> struct underlying_integral_type<5> { using type = uint64_t; };
template<> struct underlying_integral_type<6> { using type = uint64_t; };
template<> struct underlying_integral_type<7> { using type = uint64_t; };
template<> struct underlying_integral_type<8> { using type = uint64_t; };

template<typename T>
using underlying_integral_type_t = typename underlying_integral_type<sizeof(T)>::type;

} // namespace bits::detail

#endif /* BITS_UNDERLYING_INTEGRAL_TYPE_H */