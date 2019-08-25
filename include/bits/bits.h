#ifndef BITS_BITS_H
#define BITS_BITS_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <array>

#include "bits/detail/detail.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T> void insert(T val, uint8_t * buffer, size_t high, size_t low);
template<typename T> T    extract(const uint8_t * buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> void insert(T val, uint8_t * buffer);
template<typename T, size_t high, size_t low> T    extract(const uint8_t * buffer);

template<typename T, size_t N> void insert(T val, std::array<uint8_t, N> & buffer, size_t high, size_t low);
template<typename T, size_t N> T    extract(const std::array<uint8_t, N> & buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low, size_t N> void insert(T val, std::array<uint8_t, N> & buffer);
template<typename T, size_t high, size_t low, size_t N> T    extract(const std::array<uint8_t, N> & buffer);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
void insert(T val, uint8_t * buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const detail::SerializeInfos infos(high, low);

    detail::insert(val, buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
void insert(T val, uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::SerializeInfos infos(high, low);

    detail::insert(val, buffer, infos);
}

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
void insert(T val, std::array<uint8_t, N> & buffer, size_t high, size_t low)
{
    bits::insert<T>(val, buffer.data(), high, low);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low, size_t N>
void insert(T val, std::array<uint8_t, N> & buffer)
{
    bits::insert<T, high, low>(val, buffer.data());
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

#endif /* BITS_BITS_H */