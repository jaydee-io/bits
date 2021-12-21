////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_EXTRACTION_H
#define BITS_BITS_EXTRACTION_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <array>

#include <bits/detail/detail.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>                          T extract(const uint8_t * buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> T extract(const uint8_t * buffer);

template<typename T, size_t N>                          T extract(const std::array<uint8_t, N> & buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low, size_t N> T extract(const std::array<uint8_t, N> & buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::DeserializeInfos infos(sizeof(T) * 8, high, low);

    return detail::extract<T>(buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
T extract(const uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::DeserializeInfos infos(sizeof(T) * 8, high, low);

    return detail::extract<T>(buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T, size_t N>
T extract(const std::array<uint8_t, N> & buffer, size_t high, size_t low)
{
    return bits::extract<T>(buffer.data(), high, low);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low, size_t N>
T extract(const std::array<uint8_t, N> & buffer)
{
    return bits::extract<T, high, low>(buffer.data());
}

} // namespace bits

#endif /* BITS_BITS_EXTRACTION_H */