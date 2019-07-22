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

    const size_t  byte_start = detail::num_byte(low);
    const size_t  byte_end   = detail::num_byte(high);
    const uint8_t first_byte_mask =  detail::serialize_first_byte_mask_8bits(low, high);
    const size_t  first_byte_shift = detail::first_byte_shift_8bits(high);

    detail::insert(val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
void insert(T val, uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr size_t  byte_start = detail::num_byte(low);
    constexpr size_t  byte_end   = detail::num_byte(high);
    constexpr uint8_t first_byte_mask =  detail::serialize_first_byte_mask_8bits(low, high);
    constexpr size_t  first_byte_shift = detail::first_byte_shift_8bits(high);

    detail::insert(val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
}

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, size_t high, size_t low)
{
    assert((sizeof(T) * 8) >= (high - low + 1));

    const size_t  byte_start = detail::num_byte(low);
    const size_t  byte_end   = detail::num_byte(high);
    const uint8_t first_byte_mask  = detail::deserialize_first_byte_mask_8bits(low, high);
    const size_t  first_byte_shift = detail::first_byte_shift_8bits(high);
    const size_t  last_byte_shift  = detail::last_byte_shift_8bits(high);

    return detail::extract<T>(buffer, byte_start, byte_end, first_byte_mask, first_byte_shift, last_byte_shift);
}

//-----------------------------------------------------------------------------
template<typename T, size_t high, size_t low>
T extract(const uint8_t * buffer)
{
    static_assert((sizeof(T) * 8) >= (high - low + 1));

    constexpr size_t  byte_start = detail::num_byte(low);
    constexpr size_t  byte_end   = detail::num_byte(high);
    constexpr uint8_t first_byte_mask  = detail::deserialize_first_byte_mask_8bits(low, high);
    constexpr size_t  first_byte_shift = detail::first_byte_shift_8bits(high);
    constexpr size_t  last_byte_shift  = detail::last_byte_shift_8bits(high);

    return detail::extract<T>(buffer, byte_start, byte_end, first_byte_mask, first_byte_shift, last_byte_shift);
}

} // namespace bits

#endif /* BITS_BITS_H */