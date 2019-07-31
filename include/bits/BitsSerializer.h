#ifndef BITS_BITS_SERIALIZER_H
#define BITS_BITS_SERIALIZER_H

#include <stdint.h>
#include <cstdlib>

#include "bits/bits.h"
#include "bits/detail/BaseBitsStream.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::BaseBitsStream<BitsSerializer>
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline BitsSerializer & insert(T val, size_t nbBits);

protected:
    uint8_t * const buffer;
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsSerializer::BitsSerializer(uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: BaseBitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
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

} // namespace bits

#endif /* BITS_BITS_SERIALIZER_H */