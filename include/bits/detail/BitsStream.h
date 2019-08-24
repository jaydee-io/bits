#ifndef BITS_BITS_STREAM_H
#define BITS_BITS_STREAM_H

#include <stdint.h>
#include <cstdlib>
#include <stdexcept>
#include <string_view>

#include "bits/detail/BitsStreamManipulation.h"

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Bits serializer / deserializer base class for common operations
//-----------------------------------------------------------------------------
class BitsStream
{
public:
    inline BitsStream(size_t lengthBufferBits, size_t initialOffsetBits);

    inline size_t nbBitsStreamed(void);

    inline void setManipulation(const BitsStreamManipulation manip);

protected:
    inline void checkNbRemainingBits(size_t nbBits, std::string_view message);

    const size_t lengthBits;
    const size_t offsetBits;
    size_t posBits;
    size_t nbBitsNext;
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsStream::BitsStream(size_t lengthBufferBits, size_t initialOffsetBits)
: lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits), nbBitsNext(0)
{}

//-----------------------------------------------------------------------------
size_t BitsStream::nbBitsStreamed(void)
{
    return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
void BitsStream::checkNbRemainingBits(size_t nbBits, std::string_view message)
{
    if((posBits + nbBits) > lengthBits)
        throw std::out_of_range(message.data());
}

//-----------------------------------------------------------------------------
void BitsStream::setManipulation(const BitsStreamManipulation manip)
{
    switch(manip.action)
    {
        case BitsStreamManipulation::STREAM_BITS : nbBitsNext = manip.value; break;
        case BitsStreamManipulation::SKIP_BITS   : posBits += manip.value; break;
    }
}

} // namespace bits::detail

#endif /* BITS_BITS_STREAM_H */