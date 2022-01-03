////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_INSERTION_H
#define BITS_BITS_INSERTION_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <array>

#include <bits/detail/Serializer.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T>           constexpr void insert(T val, uint8_t * buffer, size_t high, size_t low);
template<typename T, size_t N> constexpr void insert(T val, std::array<uint8_t, N> & buffer, size_t high, size_t low);

template<typename T, size_t high, size_t low>           constexpr void insert(T val, uint8_t * buffer);
template<typename T, size_t high, size_t low, size_t N> constexpr void insert(T val, std::array<uint8_t, N> & buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
constexpr void insert(T val, uint8_t * buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t N>
constexpr void insert(T val, std::array<uint8_t, N> & buffer, size_t high, size_t low)
{
    assert((N * 8) >= (high - low + 1));

    bits::insert<T>(val, buffer.data(), high, low);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
constexpr void insert(T val, uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low, size_t N>
constexpr void insert(T val, std::array<uint8_t, N> & buffer)
{
    static_assert((N * 8) >= (high - low + 1));

    bits::insert<T, high, low>(val, buffer.data());
}

} // namespace bits

#endif /* BITS_BITS_INSERTION_H */