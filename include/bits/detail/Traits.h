////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_DETAIL_TRAITS_H
#define BITS_DETAIL_TRAITS_H

#include <type_traits>
#include <span>
#include <array>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Trait to check if a type is a bits declared enum type
//-----------------------------------------------------------------------------
template<typename EnumType>
struct IsBitEnum : std::false_type {};

//-----------------------------------------------------------------------------
//- Concept for output range and iterator with assigned value of the same type
//- as dereferenced iterator
//-----------------------------------------------------------------------------
template <class I>
concept output_iterator = std::output_iterator<I, std::iter_value_t<I>>;
template <class R>
concept output_range = std::ranges::output_range<R, std::ranges::range_value_t<R>>;

//-----------------------------------------------------------------------------
//- Traits to check if a type is a std::array or std::span type
//-----------------------------------------------------------------------------
template<typename T>           struct is_std_array : std::false_type {};
template<typename T, size_t N> struct is_std_array<std::array<T, N>> : std::true_type {};

template<typename T> inline constexpr bool is_std_array_v = is_std_array<T>::value;

template<typename T>           struct is_std_span : std::false_type {};
template<typename T, size_t N> struct is_std_span<std::span<T, N>> : std::true_type {};

template<typename T> inline constexpr bool is_std_span_v = is_std_span<T>::value;

} // namespace bits::detail

#endif // BITS_DETAIL_TRAITS_H
