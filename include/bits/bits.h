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

    detail::insert_last_byte         (val, buffer, byte_start, byte_end, first_byte_shift);
    detail::insert_intermediate_bytes(val, buffer, byte_start, byte_end);
    detail::insert_first_byte        (val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
}

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, size_t high, size_t low)
{
    const size_t  byte_start = detail::num_byte(low);
    const size_t  byte_end   = detail::num_byte(high);
    const uint8_t first_byte_mask  = detail::deserialize_first_byte_mask_8bits(low, high);
    const size_t  first_byte_shift = detail::first_byte_shift_8bits(high);
    const size_t  last_byte_shift  = detail::last_byte_shift_8bits(high);

    assert((sizeof(T) * 8) >= (high - low + 1));

    T val = 0;

    detail::extract_first_byte        (val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
    detail::extract_intermediate_bytes(val, buffer, byte_start, byte_end);
    detail::extract_last_byte         (val, buffer, byte_start, byte_end, last_byte_shift);

    return val;
}

} // namespace bits

#endif /* BITS_BITS_H */