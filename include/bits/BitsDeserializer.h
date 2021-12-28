////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_DESERIALIZER_H
#define BITS_BITS_DESERIALIZER_H

#include <stdint.h>
#include <cstdlib>
#include <array>

#include <bits/bits_extraction.h>
#include <bits/detail/BitsStream.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Bits deserializer class
//-----------------------------------------------------------------------------
class BitsDeserializer : public detail::BitsStream<BitsDeserializer>
{
public:
    inline BitsDeserializer(const uint8_t * buffer, size_t lengthBufferBits, size_t initialOffsetBits = 0);
    template<size_t N>
    inline BitsDeserializer(const std::array<uint8_t, N> & buffer, size_t lengthBufferBits = N * 8, size_t initialOffsetBits = 0);

    template<typename T, std::enable_if_t< ! std::is_same_v<T, BitsDeserializer>, int> = 0>
    inline T extract(size_t nbBits);
    template<typename T>
    inline BitsDeserializer & extract(T & val, size_t nbBits = sizeof(T) * 8);

protected:
    const uint8_t * const buffer;
};

template<typename T>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T & val);
inline BitsDeserializer & operator >>(BitsDeserializer & bs, const detail::BitsStreamManipulation manip);

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline BitsDeserializer::BitsDeserializer(const uint8_t * buffer_, size_t lengthBufferBits, size_t initialOffsetBits)
: BitsStream(lengthBufferBits, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<size_t N>
inline BitsDeserializer::BitsDeserializer(const std::array<uint8_t, N> & buffer, size_t lengthBufferBits, size_t initialOffsetBits)
: BitsDeserializer(buffer.data(), lengthBufferBits, initialOffsetBits)
{}

//-----------------------------------------------------------------------------
template<typename T, std::enable_if_t< ! std::is_same_v<T, BitsDeserializer>, int>>
inline T BitsDeserializer::extract(size_t nbBits)
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
    auto nbBitsToExtract = nbBitsNext ? nbBitsNext : nbBits;
    val = extract<T>(nbBitsToExtract);
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T & val)
{
    return bs.extract<T>(val, sizeof(T) * 8);
}

//-----------------------------------------------------------------------------
inline BitsDeserializer & operator >>(BitsDeserializer & bs, detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip);
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_DESERIALIZER_H */
