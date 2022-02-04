////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_DETAIL_BASE_SERIALIZATION_H
#define BITS_DETAIL_BASE_SERIALIZATION_H

#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Base class with intermediate data used by de/serialization algorithms
//-----------------------------------------------------------------------------
class BaseSerialization
{
public:
    inline constexpr BaseSerialization(size_t high, size_t low) noexcept;

protected:
    inline constexpr bool isSameByte(void) const noexcept;

    const size_t byte_start;
    const size_t byte_end;
    const size_t first_byte_shift;
};



//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline static constexpr size_t    num_byte(size_t bit)                { return bit / 8; }
inline static constexpr std::byte mask_8bits(size_t bit)              { return std::byte((1 << (8 - (bit % 8))) - 1); }
inline static constexpr std::byte mask_8bits(size_t low, size_t high) { return std::byte((1 << (((high - low) % 8) + 1)) - 1); }
inline static constexpr size_t    first_byte_shift_8bits(size_t bit)  { return 7 - (bit % 8); }

//-----------------------------------------------------------------------------
inline constexpr BaseSerialization::BaseSerialization(size_t high, size_t low) noexcept
: byte_start { detail::num_byte(low) }
, byte_end   { detail::num_byte(high) }
, first_byte_shift { detail::first_byte_shift_8bits(high) }
{}

//-----------------------------------------------------------------------------
inline constexpr bool BaseSerialization::isSameByte(void) const noexcept
{
    return byte_start == byte_end;
}

} // namespace bits::detail

#endif /* BITS_DETAIL_BASE_SERIALIZATION_H */