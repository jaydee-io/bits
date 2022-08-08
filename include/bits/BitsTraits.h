////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_TRAITS_H
#define BITS_BITS_TRAITS_H

#include <type_traits>
#include <array>
#include <string_view>
#include <bits/detail/Traits.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Traits to check if a type is
//-     - a bits declared enum type
//-     - a flags enum type
//-----------------------------------------------------------------------------
template<typename T>
inline constexpr bool is_enum_v = detail::IsBitEnum<T>::value;

template<typename T>
inline constexpr bool is_flags_bits_v = detail::IsFlagsBits<T>::value;

template<typename T>
inline constexpr bool is_flags_bits_enum_v = detail::IsFlagsBitsEnum<T>::value;

//-----------------------------------------------------------------------------
//- Utility to get enum properties :
//-     - size() : number of enum values
//-     - names() : array of all enum names
//-     - values() : array of all enum values
//-----------------------------------------------------------------------------
template<typename EnumType>
inline constexpr size_t size(void) { static_assert(bits::is_enum_v<EnumType>, "Provided type is not a bits enum type"); return 0; }
template<typename EnumType>
inline constexpr std::array<std::string_view, size<EnumType>()> names(void) { static_assert(bits::is_enum_v<EnumType>, "Provided type is not a bits enum type"); return {}; }
template<typename EnumType>
inline constexpr std::array<EnumType, size<EnumType>()> values(void) { static_assert(bits::is_enum_v<EnumType>, "Provided type is not a bits enum type"); return {}; }

} // namespace bits

#endif // BITS_BITS_TRAITS_H
