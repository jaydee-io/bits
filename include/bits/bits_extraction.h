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
#include <array>

#include <bits/detail/Deserializer.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>           constexpr T extract(const uint8_t * buffer, size_t high, size_t low);
template<typename T, size_t N> constexpr T extract(const std::array<uint8_t, N> & buffer, size_t high, size_t low);

template<typename T, size_t high, size_t low>           constexpr T extract(const uint8_t * buffer);
template<typename T, size_t high, size_t low, size_t N> constexpr T extract(const std::array<uint8_t, N> & buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
constexpr T extract(const uint8_t * buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::Deserializer deserializer(sizeof(T) * 8, high, low);

    return deserializer.extract<T>(buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t N>
constexpr T extract(const std::array<uint8_t, N> & buffer, size_t high, size_t low)
{
    return bits::extract<T>(buffer.data(), high, low);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr T extract(const uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::Deserializer deserializer(sizeof(T) * 8, high, low);

    return deserializer.extract<T>(buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low, size_t N>
constexpr T extract(const std::array<uint8_t, N> & buffer)
{
    static_assert((N * 8) >= (high - low + 1));

    return bits::extract<T, high, low>(buffer.data());
}

} // namespace bits

#endif /* BITS_BITS_EXTRACTION_H */