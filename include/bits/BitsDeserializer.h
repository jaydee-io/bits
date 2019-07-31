#ifndef BITS_BITS_DESERIALIZER_H
#define BITS_BITS_DESERIALIZER_H

#include <stdint.h>
#include <cstdlib>

#include "bits/bits.h"
#include "bits/detail/BaseBitsStream.h"

namespace bits {

//-----------------------------------------------------------------------------
//- Bits deserializer class
//-----------------------------------------------------------------------------
class BitsDeserializer : public detail::BaseBitsStream
{
public:
    inline BitsDeserializer(const uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);

    template<typename T>
    inline T extract(size_t nbBits);
    template<typename T>
    inline BitsDeserializer & extract(T & val, size_t nbBits);

protected:
    const uint8_t * const buffer;
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsDeserializer::BitsDeserializer(const uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: BaseBitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<typename T>
T BitsDeserializer::extract(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to extract bits, too few bits remaining");

    auto val = bits::extract<T>(buffer, posBits + nbBits - 1, posBits);
    posBits += nbBits;

    return val;
}

//-----------------------------------------------------------------------------
template<typename T>
inline BitsDeserializer & BitsDeserializer::extract(T & val, size_t nbBits)
{
    val = extract<T>(nbBits);

    return *this;
}

} // namespace bits

#endif /* BITS_BITS_DESERIALIZER_H */