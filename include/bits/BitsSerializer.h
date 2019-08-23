#ifndef BITS_BITS_SERIALIZER_H
#define BITS_BITS_SERIALIZER_H

#include <stdint.h>
#include <cstdlib>
#include <array>

#include "bits/bits.h"
#include "bits/detail/SkippableBitsStream.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::SkippableBitsStream<BitsSerializer>
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);
    template<size_t N>
    inline BitsSerializer(std::array<uint8_t, N> & buffer, size_t lengthBufferBits = N * 8, size_t initialOffsetBits = 0);

    template<typename T>
    inline BitsSerializer & insert(T val, size_t nbBits);
    template<typename T>
    inline BitsSerializer & insert(T val);

protected:
    uint8_t * const buffer;
};

template<typename T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsSerializer::BitsSerializer(uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: SkippableBitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<size_t N>
BitsSerializer::BitsSerializer(std::array<uint8_t, N> & buffer, size_t lengthBufferBits, size_t initialOffsetBits)
: BitsSerializer(buffer.data(), lengthBufferBits, initialOffsetBits)
{}

//-----------------------------------------------------------------------------
template<typename T>
BitsSerializer & BitsSerializer::insert(T val, size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to insert bits, too few bits remaining");

    bits::insert<T>(val, buffer, posBits + nbBits - 1, posBits);
    posBits += nbBits;

    return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
BitsSerializer & BitsSerializer::insert(T val)
{
    auto nbBits = nbBitsNext ? nbBitsNext : sizeof(T) * 8;

    insert<T>(val, nbBits);
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val)
{
    return bs.insert<T>(val);
}

//-----------------------------------------------------------------------------
template<>
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip); 
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_SERIALIZER_H */