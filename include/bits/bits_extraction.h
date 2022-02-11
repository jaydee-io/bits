////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_EXTRACTION_H
#define BITS_BITS_EXTRACTION_H

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <span>

#include <bits/detail/Deserializer.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>                          constexpr void extract(const std::span<const std::byte> buffer, T & val, size_t high, size_t low);
template<typename T, size_t high, size_t low> constexpr void extract(const std::span<const std::byte> buffer, T & val);

template<typename T>                          constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> constexpr T extract(const std::span<const std::byte> buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
constexpr void extract(const std::span<const std::byte> buffer, T & val, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::Deserializer deserializer(sizeof(T) * CHAR_BIT, high, low);

    deserializer.extract<T>(buffer, val);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr void extract(const std::span<const std::byte> buffer, T & val)
{
    assert((buffer.size() * 8) >= (high - low + 1));
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::Deserializer deserializer(sizeof(T) * 8, high, low);

    deserializer.extract<T>(buffer, val);
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low)
{
    T val;
    extract<T>(buffer, val, high, low);
    return val;
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr T extract(const std::span<const std::byte> buffer)
{
    T val;
    extract<T, high, low>(buffer, val);
    return val;
}

} // namespace bits

#endif /* BITS_BITS_EXTRACTION_H */