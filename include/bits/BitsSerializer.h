////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_SERIALIZER_H
#define BITS_BITS_SERIALIZER_H

#include <stdint.h>
#include <cstdlib>
#include <array>

#include <bits/bits_insertion.h>
#include <bits/detail/BitsStream.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::BitsStream<BitsSerializer>
{
public:
    inline BitsSerializer(uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);
    template<size_t N>
    inline BitsSerializer(std::array<uint8_t, N> & buffer, size_t lengthBufferBits = N * 8, size_t initialOffsetBits = 0);

    template<typename T>
    inline BitsSerializer & insert(T val, size_t nbBits = sizeof(T) * 8);

protected:
    template<typename T>
    inline BitsSerializer & insertUsingInternalState(T val);

    template<typename T>
    friend inline BitsSerializer & operator <<(BitsSerializer & bs, T val);
    
    uint8_t * const buffer;
};

template<typename T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val);
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsSerializer::BitsSerializer(uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: BitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
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
    auto nbBitsToInsert = nbBitsNext ? nbBitsNext : nbBits;
    
    checkNbRemainingBits(nbBitsToInsert, "Unable to insert bits, too few bits remaining");

    bits::insert<T>(val, buffer, posBits + nbBitsToInsert - 1, posBits);
    posBits += nbBitsToInsert;
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
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip); 
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_SERIALIZER_H */