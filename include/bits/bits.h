#ifndef BITS_BITS_H
#define BITS_BITS_H

#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <stdexcept>

namespace bits {

class BitsSerializer
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline void insert(T val, size_t nbBits);

    inline size_t nbBitsSerialized(void);

protected:
    uint8_t * buffer;
    size_t lengthBits;
    size_t offsetBits;
    size_t posBits;
};


//------------------------------------------------------
BitsSerializer::BitsSerializer(uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: buffer(buffer_), lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//------------------------------------------------------
size_t BitsSerializer::nbBitsSerialized(void)
{
        return posBits - offsetBits;
}

//------------------------------------------------------
static inline size_t serialize_num_byte(size_t bit) { return bit / 8; }
static inline uint8_t serialize_upper_mask_8bits(size_t bit) { return ~((1 << (8 - (bit % 8))) - 1); }
static inline uint8_t serialize_lower_mask_8bits(size_t bit) { return  ((1 << (7 - (bit % 8))) - 1); }
static inline size_t serialize_first_byte_shift_8bits(size_t bit) { return 7 - (bit % 8); }

//------------------------------------------------------
static inline uint8_t serialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(serialize_num_byte(low) == serialize_num_byte(high))
        return serialize_lower_mask_8bits(high) | serialize_upper_mask_8bits(low);
    else
        return serialize_upper_mask_8bits(low);
}

//------------------------------------------------------
template<typename T>
void BitsSerializer::insert(T val, size_t nbBits)
{
    assert((sizeof(T) * 8) >= nbBits);
    if((nbBitsSerialized() + nbBits) > lengthBits)
        throw std::overflow_error("insert, overflow_error");

    const size_t  high = posBits + nbBits - 1; // Upper bit
    const size_t  low  = posBits;              // Lower bit
    const size_t  byte_start = serialize_num_byte(low);
    const size_t  byte_end   = serialize_num_byte(high);
    const uint8_t first_byte_mask =  serialize_first_byte_mask_8bits(low, high);
    const size_t  first_byte_shift = serialize_first_byte_shift_8bits(high);

    // Last byte
    if(byte_start != byte_end)
    {
        uint8_t inserted_val = ((val << first_byte_shift) & 0xFF);
        buffer[byte_end] = buffer[byte_end] | inserted_val;
        buffer[byte_end] = buffer[byte_end] & (inserted_val | ((1 << first_byte_shift) - 1));
        val = val >> (8 - first_byte_shift);
    }

    // Intermediate bytes
    for(size_t i=byte_end ? byte_end - 1 : byte_start; i>byte_start; i--)
    {
        buffer[i] = val & 0xFF;
        val = val >> 8;
    }

    // First byte
    if(byte_start == byte_end)
        val = val << first_byte_shift;
    buffer[byte_start] = buffer[byte_start] | val;
    buffer[byte_start] = buffer[byte_start] & (val | first_byte_mask);

    // Shift the current position
    posBits += nbBits;
}



//----------------------------------------------------------------



class BitsDeserializer
{
public:
    inline BitsDeserializer(const uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline T extract(size_t nbBits);

    inline size_t nbBitsDeserialized() const;

protected:
    const uint8_t * buffer;
    size_t lengthBits;
    size_t offsetBits;
    size_t posBits;
};


//------------------------------------------------------
BitsDeserializer::BitsDeserializer(const uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: buffer(buffer_), lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//------------------------------------------------------
size_t BitsDeserializer::nbBitsDeserialized() const
{
    return posBits - offsetBits;
}

//------------------------------------------------------
static inline size_t deserialize_num_byte(size_t bit) { return bit / 8; }
static inline uint8_t deserialize_mask_8bits(size_t low, size_t high) { return  (1 << (((high - low) % 8) + 1)) - 1; }
static inline uint8_t deserialize_mask_8bits(size_t bit) { return  (1 << (8 - (bit % 8))) - 1; }
static inline size_t deserialize_first_byte_shift_8bits(size_t bit) { return 7 - (bit % 8); }
static inline size_t deserialize_last_byte_shift_8bits(size_t bit) { return (bit % 8) + 1; }

//------------------------------------------------------
static inline uint8_t deserialize_first_byte_mask_8bits(size_t low, size_t high)
{
    if(serialize_num_byte(low) == serialize_num_byte(high))
        return deserialize_mask_8bits(low, high);
    else
        return deserialize_mask_8bits(low);
}

//------------------------------------------------------
template<typename T>
T BitsDeserializer::extract(size_t nbBits)
{
    const size_t  high = posBits + nbBits - 1; // Upper bit
    const size_t  low  = posBits;              // Lower bit
    const size_t  byte_start = deserialize_num_byte(low);
    const size_t  byte_end   = deserialize_num_byte(high);
    const uint8_t first_byte_mask =  deserialize_first_byte_mask_8bits(low, high);
    const size_t  first_byte_shift = deserialize_first_byte_shift_8bits(high);
    const size_t  last_byte_shift = deserialize_last_byte_shift_8bits(high);

    assert((sizeof(T) * 8) >= (high - low + 1));
    if(posBits + nbBits > lengthBits)
        throw std::out_of_range("extract, out_of_range");

    T val = 0;

    // First byte
    val = buffer[byte_start];
    if(byte_start == byte_end)
        val = val >> first_byte_shift;
    val = val & first_byte_mask;

    // Intermediate bytes
    for(size_t i=byte_start + 1; i<byte_end; i++)
        val = (val << 8) | buffer[i];

    // Last byte
    if(byte_start != byte_end)
        val = (val << last_byte_shift) | (buffer[byte_end] >> (8 - last_byte_shift));

    // Shift current position
    posBits += nbBits;

    return val;
}

} // namespace bits

#endif /* BITS_BITS_H */