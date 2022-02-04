////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_INSERTION_H
#define BITS_BITS_INSERTION_H

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <span>

#include <bits/detail/Serializer.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>                          constexpr void insert(T val, const std::span<std::byte> buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> constexpr void insert(T val, const std::span<std::byte> buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
constexpr void insert(T val, const std::span<std::byte> buffer, size_t high, size_t low)
{
    assert((buffer.size() * CHAR_BIT) >= (high - low + 1));
    assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    const detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr void insert(T val, const std::span<std::byte> buffer)
{
    assert((buffer.size() * CHAR_BIT) >= (high - low + 1));
    static_assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    constexpr detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

} // namespace bits

#endif /* BITS_BITS_INSERTION_H */