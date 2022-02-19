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
#include <climits>
#include <span>

#include <bits/bits_extraction.h>
#include <bits/detail/Traits.h>
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
    requires(not std::is_same_v<T, BitsDeserializer>)
    inline T extract(size_t nbBits = sizeof(T) * CHAR_BIT);
    template<typename T>
    inline BitsDeserializer & extract(T && val, size_t nbBits = sizeof(T) * CHAR_BIT);
    template<detail::output_range R>
    inline BitsDeserializer & extract(R && r, size_t nbBits = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

protected:
    const std::span<const std::byte> buffer;
};

template<typename T>
requires(not detail::output_range<T>)
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T && val);
template<detail::output_range R>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, R && r);
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
requires(not std::is_same_v<T, BitsDeserializer>)
inline T BitsDeserializer::extract(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to extract bits, too few bits remaining");

    auto val = bits::extract<T>(buffer, posBits + nbBits - 1, posBits);
    posBits += nbBits;

    return val;
}

//-----------------------------------------------------------------------------
template<typename T>
inline BitsDeserializer & BitsDeserializer::extract(T && val, size_t nbBits)
{
    auto nbBitsToExtract = nbBitsNext ? nbBitsNext : nbBits;
    checkNbRemainingBits(nbBitsToExtract, "Unable to extract bits, too few bits remaining");

    bits::extract(buffer, std::forward<T>(val), posBits + nbBitsToExtract - 1, posBits);
    posBits += nbBitsToExtract;
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
namespace detail {

template<detail::output_range R>
inline constexpr size_t range_size(R && r)
{
    if constexpr(std::ranges::sized_range<R>)
        return std::ranges::size(std::forward<R>(r));
    else
        return std::ranges::distance(std::ranges::begin(std::forward<R>(r)), std::ranges::end(std::forward<R>(r)));
}

} // namespace detail

//-----------------------------------------------------------------------------
template<detail::output_range R>
inline BitsDeserializer & BitsDeserializer::extract(R && r, size_t nbBits)
{
    auto nbBitsToExtractByElement = nbBitsNext ? nbBitsNext : nbBits;
    auto nbBitsToExtract = nbBitsToExtractByElement * detail::range_size(std::forward<R>(r));
    checkNbRemainingBits(nbBitsToExtract, "Unable to extract bits, too few bits remaining");

    bits::extract(buffer, std::forward<R>(r), posBits + nbBitsToExtract - 1, posBits, nbBitsToExtractByElement);
    posBits += nbBitsToExtract;
    nbBitsNext = 0;

    return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
requires(not detail::output_range<T>)
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T && val)
{
    return bs.extract(std::forward<T>(val), sizeof(T) * CHAR_BIT);
}

//-----------------------------------------------------------------------------
template<detail::output_range R>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, R && r)
{
    return bs.extract(std::forward<R>(r), sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);
}

//-----------------------------------------------------------------------------
inline BitsDeserializer & operator >>(BitsDeserializer & bs, detail::BitsStreamManipulation manip)
{
    bs.setManipulation(manip);
    return bs;
}

} // namespace bits

#endif /* BITS_BITS_DESERIALIZER_H */
