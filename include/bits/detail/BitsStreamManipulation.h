////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_STREAM_MANIPULATION_H
#define BITS_BITS_STREAM_MANIPULATION_H

namespace bits {

namespace detail {
//-----------------------------------------------------------------------------
//- Type used to store the state of a BitsStream manipulation :
//-----------------------------------------------------------------------------
struct BitsStreamManipulation
{
    enum class Action {
        STREAM_BITS,
        SKIP_BITS,
        RESET,
    };
    
    Action action;
    size_t value = 0;
};

} // namespace bits::detail

//-----------------------------------------------------------------------------
//- BitsStream manipulation functions :
//-     - set the number of bits to insert / extract
//-     - set the number of bits to skip
//-     - reset stream to begining
//-----------------------------------------------------------------------------
inline detail::BitsStreamManipulation nbits(size_t nbBits)
{
    return { detail::BitsStreamManipulation::Action::STREAM_BITS, nbBits };
}

inline detail::BitsStreamManipulation skip(size_t nbBits)
{
    return { detail::BitsStreamManipulation::Action::SKIP_BITS, nbBits };
}

inline detail::BitsStreamManipulation reset(void)
{
    return { detail::BitsStreamManipulation::Action::RESET, 0 };
}

} // namespace bits

#endif /* BITS_BITS_STREAM_MANIPULATION_H */