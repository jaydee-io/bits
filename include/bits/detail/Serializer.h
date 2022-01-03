////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_DETAIL_SERIALIZER_H
#define BITS_DETAIL_SERIALIZER_H

#include <bits/detail/BaseSerialization.h>
#include <bits/detail/underlying_integral_type.h>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Internal implementation of serialization algorithm
//-----------------------------------------------------------------------------
class Serializer : public BaseSerialization
{
public:
    constexpr Serializer(size_t high, size_t low) noexcept;

    template<typename T> constexpr void insert(T val, uint8_t * buffer) const noexcept;

private:
    constexpr uint8_t serialize_first_byte_mask_8bits(size_t low, size_t high) const noexcept;

    template<typename T> constexpr void insert_last_byte(T & val, uint8_t * buffer) const noexcept;
    template<typename T> constexpr void insert_intermediate_bytes(T & val, uint8_t * buffer) const noexcept;
    template<typename T> constexpr void insert_first_byte(T & val, uint8_t * buffer) const noexcept;

    const uint8_t first_byte_mask;
};



//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline static constexpr uint8_t lower_mask_8bits(size_t bit) { return (1 << (7 - (bit % 8))) - 1; }
inline static constexpr uint8_t upper_mask_8bits(size_t bit) { return ~mask_8bits(bit); }

//-----------------------------------------------------------------------------
constexpr Serializer::Serializer(size_t high, size_t low) noexcept
: BaseSerialization(high, low)
, first_byte_mask { serialize_first_byte_mask_8bits(low, high) }
{}

//-----------------------------------------------------------------------------
template<typename T>
void constexpr Serializer::insert(T val, uint8_t * buffer) const noexcept
{
    auto rawVal = static_cast<underlying_integral_type_t<T>>(val);

    insert_last_byte         (rawVal, buffer);
    insert_intermediate_bytes(rawVal, buffer);
    insert_first_byte        (rawVal, buffer);
}

//-----------------------------------------------------------------------------
constexpr uint8_t Serializer::serialize_first_byte_mask_8bits(size_t low, size_t high) const noexcept
{
    if(isSameByte())
        return lower_mask_8bits(high) | upper_mask_8bits(low);
    else
        return upper_mask_8bits(low);
}

//-----------------------------------------------------------------------------
template<typename T>
void constexpr Serializer::insert_last_byte(T & val, uint8_t * buffer) const noexcept
{
    if(not isSameByte())
    {
        uint8_t inserted_val = ((val << first_byte_shift) & 0xFF);
        buffer[byte_end] |= inserted_val;
        buffer[byte_end] &= (inserted_val | ((1 << first_byte_shift) - 1));
        val = val >> (8 - first_byte_shift);
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void constexpr Serializer::insert_intermediate_bytes(T & val, uint8_t * buffer) const noexcept
{
    for(size_t i=byte_end ? byte_end - 1 : byte_start; i>byte_start; i--)
    {
        buffer[i] = val & 0xFF;
        val = val >> 8;
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void constexpr Serializer::insert_first_byte(T & val, uint8_t * buffer) const noexcept
{
    if(isSameByte())
        val = (val << first_byte_shift) & ~first_byte_mask;
    buffer[byte_start] |= val;
    buffer[byte_start] &= (val | first_byte_mask);
}

} // namespace bits::detail

#endif /* BITS_DETAIL_SERIALIZER_H */