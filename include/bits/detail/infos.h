#ifndef BITS_INFOS_H
#define BITS_INFOS_H

#include <stdint.h>
#include <cstdlib>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Internal data used by serialization algorithm
//-----------------------------------------------------------------------------
struct SerializeInfos
{
    constexpr SerializeInfos(size_t high, size_t low);

    const size_t  byte_start;
    const size_t  byte_end;
    const uint8_t first_byte_mask;
    const size_t  first_byte_shift;
};

//-----------------------------------------------------------------------------
//- Internal data used by deserialization algorithm
//-----------------------------------------------------------------------------
struct DeserializeInfos
{
    constexpr DeserializeInfos(size_t type_size, size_t high, size_t low);

    const size_t  byte_start;
    const size_t  byte_end;
    const uint8_t first_byte_mask;
    const size_t  first_byte_shift;
    const size_t  last_byte_shift;
    const size_t  sign_shift;
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline constexpr size_t  num_byte(size_t bit)                { return bit / 8; }
inline constexpr uint8_t mask_8bits(size_t low, size_t high) { return (1 << (((high - low) % 8) + 1)) - 1; }
inline constexpr uint8_t mask_8bits(size_t bit)              { return (1 << (8 - (bit % 8))) - 1; }
inline constexpr uint8_t lower_mask_8bits(size_t bit)        { return (1 << (7 - (bit % 8))) - 1; }
inline constexpr uint8_t upper_mask_8bits(size_t bit)        { return ~mask_8bits(bit); }
inline constexpr size_t  first_byte_shift_8bits(size_t bit)  { return 7 - (bit % 8); }
inline constexpr size_t  last_byte_shift_8bits(size_t bit)   { return     (bit % 8) + 1; }

//-----------------------------------------------------------------------------
inline constexpr uint8_t serialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(num_byte(low) == num_byte(high))
        return lower_mask_8bits(high) | upper_mask_8bits(low);
    else
        return upper_mask_8bits(low);
}

//-----------------------------------------------------------------------------
inline constexpr uint8_t deserialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(num_byte(low) == num_byte(high))
        return mask_8bits(low, high);
    else
        return mask_8bits(low);
}

//-----------------------------------------------------------------------------
constexpr SerializeInfos::SerializeInfos(size_t high, size_t low)
: byte_start { detail::num_byte(low) }
, byte_end   { detail::num_byte(high) }
, first_byte_mask  { detail::serialize_first_byte_mask_8bits(low, high) }
, first_byte_shift { detail::first_byte_shift_8bits(high) }
{}

//-----------------------------------------------------------------------------
constexpr DeserializeInfos::DeserializeInfos(size_t type_size, size_t high, size_t low)
: byte_start { detail::num_byte(low) }
, byte_end   { detail::num_byte(high) }
, first_byte_mask  { detail::deserialize_first_byte_mask_8bits(low, high) }
, first_byte_shift { detail::first_byte_shift_8bits(high) }
, last_byte_shift  { detail::last_byte_shift_8bits(high) }
, sign_shift { type_size - high + low -1 }
{}

} // namespace bits::detail

#endif /* BITS_INFOS_H */