////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_TRAITS_H
#define BITS_BITS_TRAITS_H

#include <type_traits>
#include <bits/detail/Traits.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Traits to check if a type is a bits declared enum type
//-----------------------------------------------------------------------------
template<typename T>
inline constexpr bool is_enum_v = detail::IsBitEnum<T>::value;

//-----------------------------------------------------------------------------
//- Utility traits to get enum properties :
//-     - size() : number of enum values
//-----------------------------------------------------------------------------
template<typename EnumType>
struct EnumTraits : std::false_type
{
    inline static constexpr size_t size(void);

    static_assert(bits::is_enum_v<EnumType>, "Provided type is not a bits enum type");
};

} // namespace bits

#endif // BITS_BITS_TRAITS_H
