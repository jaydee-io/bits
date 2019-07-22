#ifndef BITS_BITS_H
#define BITS_BITS_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>

#include "bits/detail/detail.h"
#include "bits/detail/BaseBitsStream.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T> void insert(T val, uint8_t * buffer, size_t high, size_t low);
template<typename T> T    extract(const uint8_t * buffer, size_t high, size_t low);
template<typename T, size_t high, size_t low> void insert(T val, uint8_t * buffer);
template<typename T, size_t high, size_t low> T    extract(const uint8_t * buffer);

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

    const detail::DeserializeInfos infos(high, low);

    return detail::extract<T>(buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
T extract(const uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr detail::DeserializeInfos infos(high, low);

    return detail::extract<T>(buffer, infos);
}

} // namespace bits

#endif /* BITS_BITS_H */