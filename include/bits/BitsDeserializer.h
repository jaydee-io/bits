////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_DESERIALIZER_H
#define BITS_BITS_DESERIALIZER_H

#include <cstddef>
#include <cstdlib>
#include <span>

#include <bits/bits_extraction.h>
#include <bits/detail/BitsStream.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Bits deserializer class
//-----------------------------------------------------------------------------
class BitsDeserializer : public detail::BitsStream<BitsDeserializer>
{
public:
    inline BitsDeserializer(const std::span<const std::byte> buffer, size_t initialOffsetBits = 0);

    template<typename T>
    requires (not std::is_same_v<T, BitsDeserializer>)
    inline T extract(size_t nbBits = sizeof(T) * CHAR_BIT);
    template<typename T>
    inline BitsDeserializer & extract(T & val, size_t nbBits = sizeof(T) * CHAR_BIT);

protected:
    const std::span<const std::byte> buffer;
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
inline BitsDeserializer::BitsDeserializer(const std::span<const std::byte> buffer_, size_t initialOffsetBits)
: BitsStream(buffer_.size() * CHAR_BIT, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<typename T>
requires (not std::is_same_v<T, BitsDeserializer>)
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
    return bs.extract<T>(val, sizeof(T) * CHAR_BIT);
}

//-----------------------------------------------------------------------------
inline BitsDeserializer & operator >>(BitsDeserializer & bs, detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip);
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_DESERIALIZER_H */
