#ifndef BITS_BITS_H
#define BITS_BITS_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <stdexcept>

#include "bits/detail/detail.h"
#include "bits/detail/BaseBitsStream.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
template<typename T> void insert(T val, uint8_t * buffer, size_t high, size_t low);
template<typename T> T    extract(const uint8_t * buffer, size_t high, size_t low);

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::BaseBitsStream
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline void insert(T val, size_t nbBits);

protected:
    uint8_t * const buffer;
};

//-----------------------------------------------------------------------------
//- Bits deserializer class
//-----------------------------------------------------------------------------
class BitsDeserializer : public detail::BaseBitsStream
{
public:
    inline BitsDeserializer(const uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline T extract(size_t nbBits);

protected:
    const uint8_t * const buffer;
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
: BaseBitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<typename T>
void BitsSerializer::insert(T val, size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to insert bits, too few bits remaining");

    bits::insert<T>(val, buffer, posBits + nbBits - 1, posBits);

    posBits += nbBits;
}

//-----------------------------------------------------------------------------
BitsDeserializer::BitsDeserializer(const uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: BaseBitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<typename T>
T BitsDeserializer::extract(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to extract bits, too few bits remaining");

    posBits += nbBits;

    return bits::extract<T>(buffer, posBits + nbBits - 1, posBits);
}

} // namespace bits

#endif /* BITS_BITS_H */