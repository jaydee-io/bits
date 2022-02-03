////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_EXTRACTION_H
#define BITS_BITS_EXTRACTION_H

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <span>

#include <bits/detail/Deserializer.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>                          constexpr T extract(const std::span<const uint8_t> buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> constexpr T extract(const std::span<const uint8_t> buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
constexpr T extract(const std::span<const uint8_t> buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::Deserializer deserializer(sizeof(T) * CHAR_BIT, high, low);

    return deserializer.extract<T>(buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr T extract(const std::span<const uint8_t> buffer)
{
    assert((buffer.size() * 8) >= (high - low + 1));
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::Deserializer deserializer(sizeof(T) * 8, high, low);

    return deserializer.extract<T>(buffer);
}

} // namespace bits

#endif /* BITS_BITS_EXTRACTION_H */