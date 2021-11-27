////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_SKIPPABLE_BITS_STREAM_H
#define BITS_SKIPPABLE_BITS_STREAM_H

#include <stdint.h>

#include "bits/detail/BitsStream.h"

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Bits serializer / deserializer base class for common operations
//-----------------------------------------------------------------------------
template<typename T>
class SkippableBitsStream : public BitsStream
{
public:
    using BitsStream::BitsStream;

    inline T & skip(size_t nbBits);
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
T & SkippableBitsStream<T>::skip(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to skip bits, too few bits remaining");

    posBits += nbBits;

    return static_cast<T &>(*this);
}

} // namespace bits::detail

#endif /* BITS_SKIPPABLE_BITS_STREAM_H */