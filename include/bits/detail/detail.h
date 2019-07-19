#ifndef BITS_DETAIL_H
#define BITS_DETAIL_H

#include <stdint.h>
#include <cstdlib>

namespace bits::detail {

//------------------------------------------------------
inline size_t  num_byte(size_t bit)                { return bit / 8; }
inline uint8_t mask_8bits(size_t low, size_t high) { return (1 << (((high - low) % 8) + 1)) - 1; }
inline uint8_t mask_8bits(size_t bit)              { return (1 << (8 - (bit % 8))) - 1; }
inline uint8_t lower_mask_8bits(size_t bit)        { return (1 << (7 - (bit % 8))) - 1; }
inline uint8_t upper_mask_8bits(size_t bit)        { return ~mask_8bits(bit); }
inline size_t  first_byte_shift_8bits(size_t bit)  { return 7 - (bit % 8); }
inline size_t  last_byte_shift_8bits(size_t bit)   { return     (bit % 8) + 1; }

//------------------------------------------------------
inline uint8_t serialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(num_byte(low) == num_byte(high))
        return lower_mask_8bits(high) | upper_mask_8bits(low);
    else
        return upper_mask_8bits(low);
}

//------------------------------------------------------
inline uint8_t deserialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(num_byte(low) == num_byte(high))
        return mask_8bits(low, high);
    else
        return mask_8bits(low);
}


} // namespace bits::detail

#endif /* BITS_DETAIL_H */