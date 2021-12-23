////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_STREAM_H
#define BITS_BITS_STREAM_H

#include <stdint.h>
#include <cstdlib>
#include <stdexcept>
#include <string_view>

#include <bits/detail/BitsStreamManipulation.h>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Bits serializer / deserializer base class for common operations
//-----------------------------------------------------------------------------
template<typename T>
class BitsStream
{
public:
    inline BitsStream(size_t lengthBufferBits, size_t initialOffsetBits);

    inline size_t nbBitsStreamed(void);

    inline T & skip(size_t nbBits);
    inline T & reset(void);

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
template<typename T>
BitsStream<T>::BitsStream(size_t lengthBufferBits, size_t initialOffsetBits)
: lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits), nbBitsNext(0)
{}

//-----------------------------------------------------------------------------
template<typename T>
size_t BitsStream<T>::nbBitsStreamed(void)
{
    return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
template<typename T>
T & BitsStream<T>::skip(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to skip bits, too few bits remaining");

    posBits += nbBits;

    return static_cast<T &>(*this);
}

//-----------------------------------------------------------------------------
template<typename T>
T & BitsStream<T>::reset(void)
{
    posBits = 0;
    nbBitsNext = 0;

    return static_cast<T &>(*this);
}

//-----------------------------------------------------------------------------
template<typename T>
void BitsStream<T>::checkNbRemainingBits(size_t nbBits, std::string_view message)
{
    if((posBits + nbBits) > lengthBits)
        throw std::out_of_range(message.data());
}

//-----------------------------------------------------------------------------
template<typename T>
void BitsStream<T>::setManipulation(const BitsStreamManipulation manip)
{
    switch(manip.action)
    {
        case BitsStreamManipulation::Action::STREAM_BITS : nbBitsNext = manip.value; break;
        case BitsStreamManipulation::Action::SKIP_BITS   : posBits += manip.value; break;
        case BitsStreamManipulation::Action::RESET       : posBits = offsetBits; nbBitsNext = 0; break;
    }
}

} // namespace bits::detail

#endif /* BITS_BITS_STREAM_H */