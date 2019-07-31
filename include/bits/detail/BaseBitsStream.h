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
template<typename T>
class BaseBitsStream
{
public:
    inline BaseBitsStream(size_t lengthBufferBits, size_t initialOffsetBits);

    inline T & skip(size_t nbBits);

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
template<typename T>
BaseBitsStream<T>::BaseBitsStream(size_t lengthBufferBits, size_t initialOffsetBits)
: lengthBits(lengthBufferBits), offsetBits(initialOffsetBits), posBits(initialOffsetBits)
{}

//-----------------------------------------------------------------------------
template<typename T>
size_t BaseBitsStream<T>::nbBitsStreamed(void)
{
    return posBits - offsetBits;
}

//-----------------------------------------------------------------------------
template<typename T>
T & BaseBitsStream<T>::skip(size_t nbBits)
{
    checkNbRemainingBits(nbBits, "Unable to skip bits, too few bits remaining");

    posBits += nbBits;

    return static_cast<T &>(*this);
}

//-----------------------------------------------------------------------------
template<typename T>
void BaseBitsStream<T>::checkNbRemainingBits(size_t nbBits, std::string_view message)
{
    if((posBits + nbBits) > lengthBits)
        throw std::out_of_range(message.data());
}

} // namespace bits::detail

#endif /* BITS_BASE_BITS_STREAM_H */