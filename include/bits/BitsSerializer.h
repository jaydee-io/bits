////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_SERIALIZER_H
#define BITS_BITS_SERIALIZER_H

#include <cstdint>
#include <cstdlib>
#include <span>

#include <bits/bits_insertion.h>
#include <bits/detail/BitsStream.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::BitsStream<BitsSerializer>
{
public:
    inline BitsSerializer(const std::span<uint8_t> buffer, size_t initialOffsetBits = 0);

    template<typename T>
    inline BitsSerializer & insert(T val, size_t nbBits = sizeof(T) * 8);

protected:
    template<typename T>
    inline BitsSerializer & insertUsingInternalState(T val);

    template<typename T>
    friend inline BitsSerializer & operator <<(BitsSerializer & bs, T val);

    const std::span<uint8_t> buffer;
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
BitsSerializer::BitsSerializer(const std::span<uint8_t> buffer_, size_t initialOffsetBits)
: BitsStream(buffer_.size() * CHAR_BIT, initialOffsetBits), buffer(buffer_)
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