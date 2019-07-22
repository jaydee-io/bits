#ifndef BITS_BASE_BITS_STREAM_H
#define BITS_BASE_BITS_STREAM_H

#include <stdint.h>
#include <cstdlib>
#include <stdexcept>
#include <string_view>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Bits serializer / deserializer base class for common operations
//-----------------------------------------------------------------------------
class BaseBitsStream
{
public:
    inline BaseBitsStream(size_t lengthBufferBits, size_t initialOffsetBits);

    inline size_t nbBitsStreamed(void);

protected:
    inline void checkNbRemainingBits(size_t nbBits, std::string_view message);

    const size_t lengthBits;
    const size_t offsetBits;
    size_t posBits;
};

//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BaseBitsStream::BaseBitsStream(size_t lengthBufferBits, size_t initialOffsetBits)
: lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//-----------------------------------------------------------------------------
size_t BaseBitsStream::nbBitsStreamed(void)
{
    return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
void BaseBitsStream::checkNbRemainingBits(size_t nbBits, std::string_view message)
{
    if((posBits + nbBits) > lengthBits)
        throw std::out_of_range(message.data());
}

} // namespace bits::detail

#endif /* BITS_BASE_BITS_STREAM_H */