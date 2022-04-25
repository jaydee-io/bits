////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_SERIALIZER_H
#define BITS_BITS_SERIALIZER_H

#include <cstddef>
#include <cstdlib>
#include <climits>
#include <span>

#include <bits/bits_insertion.h>
#include <bits/detail/Traits.h>
#include <bits/detail/BitsStream.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Bits serializer class
//-----------------------------------------------------------------------------
class BitsSerializer : public detail::BitsStream<BitsSerializer>
{
public:
    inline BitsSerializer(const std::span<std::byte> buffer, size_t initialOffsetBits = 0);

    template<detail::input_basic_type T>
    inline BitsSerializer & insert(T val, size_t nbBits = sizeof(T) * CHAR_BIT);
    template<std::ranges::input_range R>
    inline BitsSerializer & insert(R && r, size_t nbBits = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

protected:
    const std::span<std::byte> buffer;
};

template<detail::input_basic_type T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val);
template<std::ranges::input_range R>
inline BitsSerializer & operator <<(BitsSerializer & bs, R && r);
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip);





//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BitsSerializer::BitsSerializer(const std::span<std::byte> buffer_, size_t initialOffsetBits)
: BitsStream(buffer_.size() * CHAR_BIT, initialOffsetBits), buffer(buffer_)
{}

//-----------------------------------------------------------------------------
template<detail::input_basic_type T>
BitsSerializer & BitsSerializer::insert(T val, size_t nbBits)
{
    auto nbBitsToInsert = nbBitsNext ? nbBitsNext : nbBits;

    checkNbRemainingBits(nbBitsToInsert, "Unable to insert bits, too few bits remaining");

    bits::insert(buffer, val, posBits + nbBitsToInsert - 1, posBits);
    posBits += nbBitsToInsert;
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
template<std::ranges::input_range R>
inline BitsSerializer & BitsSerializer::insert(R && r, size_t nbBits)
{
    auto nbBitsToInsertByElement = nbBitsNext ? nbBitsNext : nbBits;
    auto nbBitsToInsert = nbBitsToInsertByElement * detail::range_size(std::forward<R>(r));
    checkNbRemainingBits(nbBitsToInsert, "Unable to insert bits, too few bits remaining");

    bits::insert(buffer, std::forward<R>(r), posBits + nbBitsToInsert - 1, posBits, nbBitsToInsertByElement);
    posBits += nbBitsToInsert;
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
template<detail::input_basic_type T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val)
{
    return bs.insert(val);
}

//-----------------------------------------------------------------------------
template<std::ranges::input_range R>
inline BitsSerializer & operator <<(BitsSerializer & bs, R && r)
{
    return bs.insert(std::forward<R>(r), sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);
}

//-----------------------------------------------------------------------------
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip);
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_SERIALIZER_H */