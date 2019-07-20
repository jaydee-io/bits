#ifndef BITS_BITS_H
#define BITS_BITS_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <stdexcept>

#include "bits/detail/detail.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T> void insert(T val, uint8_t * buffer, size_t high, size_t low);
template<typename T> T    extract(const uint8_t * buffer, size_t high, size_t low);

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline void insert(T val, size_t nbBits);

    inline size_t nbBitsSerialized(void);

protected:
    uint8_t * const buffer;
    const size_t lengthBits;
    const size_t offsetBits;
    size_t posBits;
};

//-----------------------------------------------------------------------------
//- Bits deserializer class
//-----------------------------------------------------------------------------
class BitsDeserializer
{
public:
    inline BitsDeserializer(const uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline T extract(size_t nbBits);

    inline size_t nbBitsDeserialized() const;

protected:
    const uint8_t * const buffer;
    const size_t lengthBits;
    const size_t offsetBits;
    size_t posBits;
};

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

//-----------------------------------------------------------------------------
BitsSerializer::BitsSerializer(uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: buffer(buffer_), lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//-----------------------------------------------------------------------------
size_t BitsSerializer::nbBitsSerialized(void)
{
        return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
template<typename T>
void BitsSerializer::insert(T val, size_t nbBits)
{
    if((posBits + nbBits) > lengthBits)
        throw std::overflow_error("insert, overflow_error");

    const size_t  high = posBits + nbBits - 1; // Upper bit
    const size_t  low  = posBits;              // Lower bit

    bits::insert<T>(val, buffer, high, low);

    // Shift the current position
    posBits += nbBits;
}

//-----------------------------------------------------------------------------
BitsDeserializer::BitsDeserializer(const uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: buffer(buffer_), lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//-----------------------------------------------------------------------------
size_t BitsDeserializer::nbBitsDeserialized() const
{
    return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
template<typename T>
T BitsDeserializer::extract(size_t nbBits)
{
    if((posBits + nbBits) > lengthBits)
        throw std::out_of_range("extract, out_of_range");

    const size_t  high = posBits + nbBits - 1; // Upper bit
    const size_t  low  = posBits;              // Lower bit

    // Shift current position
    posBits += nbBits;

    return bits::extract<T>(buffer, high, low);
}

} // namespace bits

#endif /* BITS_BITS_H */