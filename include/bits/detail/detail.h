#ifndef BITS_DETAIL_H
#define BITS_DETAIL_H

#include <stdint.h>
#include <cstdlib>

namespace bits::detail {

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
template<typename T>
void insert_last_byte(T & val, uint8_t * buffer, size_t byte_start, size_t byte_end, size_t first_byte_shift)
{
    if(byte_start != byte_end)
    {
        uint8_t inserted_val = ((val << first_byte_shift) & 0xFF);
        buffer[byte_end] = buffer[byte_end] | inserted_val;
        buffer[byte_end] = buffer[byte_end] & (inserted_val | ((1 << first_byte_shift) - 1));
        val = val >> (8 - first_byte_shift);
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void insert_intermediate_bytes(T & val, uint8_t * buffer, size_t byte_start, size_t byte_end)
{
    for(size_t i=byte_end ? byte_end - 1 : byte_start; i>byte_start; i--)
    {
        buffer[i] = val & 0xFF;
        val = val >> 8;
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void insert_first_byte(T & val, uint8_t * buffer, size_t byte_start, size_t byte_end, uint8_t first_byte_mask, size_t first_byte_shift)
{
    if(byte_start == byte_end)
        val = val << first_byte_shift;
    buffer[byte_start] = buffer[byte_start] | val;
    buffer[byte_start] = buffer[byte_start] & (val | first_byte_mask);
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_first_byte(T & val, const uint8_t * buffer, size_t byte_start, size_t byte_end, uint8_t first_byte_mask, size_t first_byte_shift)
{
    val = buffer[byte_start];
    if(byte_start == byte_end)
        val = val >> first_byte_shift;
    val = val & first_byte_mask;
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_intermediate_bytes(T & val, const uint8_t * buffer, size_t byte_start, size_t byte_end)
{
    for(size_t i=byte_start + 1; i<byte_end; i++)
        val = (val << 8) | buffer[i];
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_last_byte(T & val, const uint8_t * buffer, size_t byte_start, size_t byte_end, size_t last_byte_shift)
{
    if(byte_start != byte_end)
        val = (val << last_byte_shift) | (buffer[byte_end] >> (8 - last_byte_shift));
}

//-----------------------------------------------------------------------------
template<typename T>
void insert(T val, uint8_t * buffer, size_t byte_start, size_t byte_end, size_t first_byte_mask, size_t first_byte_shift)
{
    detail::insert_last_byte         (val, buffer, byte_start, byte_end, first_byte_shift);
    detail::insert_intermediate_bytes(val, buffer, byte_start, byte_end);
    detail::insert_first_byte        (val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
}

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, size_t byte_start, size_t byte_end, size_t first_byte_mask, size_t first_byte_shift, size_t last_byte_shift)
{
    T val = 0;

    detail::extract_first_byte        (val, buffer, byte_start, byte_end, first_byte_mask, first_byte_shift);
    detail::extract_intermediate_bytes(val, buffer, byte_start, byte_end);
    detail::extract_last_byte         (val, buffer, byte_start, byte_end, last_byte_shift);

    return val;
}

} // namespace bits::detail

#endif /* BITS_DETAIL_H */