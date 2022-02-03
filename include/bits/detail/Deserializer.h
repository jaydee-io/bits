////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_DETAIL_DESERIALIZER_H
#define BITS_DETAIL_DESERIALIZER_H

#include <bits/detail/BaseSerialization.h>
#include <bits/detail/underlying_integral_type.h>
#include <span>

namespace bits::detail {

//-----------------------------------------------------------------------------
//- Internal implementation of deserialization algorithm
//-----------------------------------------------------------------------------
class Deserializer : public BaseSerialization
{
public :
    constexpr Deserializer(size_t type_size, size_t high, size_t low) noexcept;

    template<typename T> constexpr T extract(const std::span<const uint8_t> buffer) const noexcept;

private:
    constexpr uint8_t deserialize_first_byte_mask_8bits(size_t low, size_t high) const noexcept;

    template<typename T> constexpr void extract_first_byte(T & val, const std::span<const uint8_t> buffer) const noexcept;
    template<typename T> constexpr void extract_intermediate_bytes(T & val, const std::span<const uint8_t> buffer) const noexcept;
    template<typename T> constexpr void extract_last_byte(T & val, const std::span<const uint8_t> buffer) const noexcept;
    template<typename T> constexpr void extend_sign(T & val) const noexcept;

    const uint8_t first_byte_mask;
    const size_t  last_byte_shift;
    const size_t  sign_shift;
};



//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline static constexpr size_t  last_byte_shift_8bits(size_t bit) { return     (bit % 8) + 1; }

//-----------------------------------------------------------------------------
constexpr Deserializer::Deserializer(size_t type_size, size_t high, size_t low) noexcept
: BaseSerialization(high, low)
, first_byte_mask  { deserialize_first_byte_mask_8bits(low, high) }
, last_byte_shift  { last_byte_shift_8bits(high) }
, sign_shift { type_size - high + low -1 }
{}

//-----------------------------------------------------------------------------
constexpr uint8_t Deserializer::deserialize_first_byte_mask_8bits(size_t low, size_t high) const noexcept
{
    if(isSameByte())
        return mask_8bits(low, high);
    else
        return mask_8bits(low);
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr T Deserializer::extract(const std::span<const uint8_t> buffer) const noexcept
{
    underlying_integral_type_t<T> val = {};

    extract_first_byte        (val, buffer);
    extract_intermediate_bytes(val, buffer);
    extract_last_byte         (val, buffer);

    if constexpr(std::is_signed_v<T>)
        extend_sign(val);

    return static_cast<T>(val);
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr void Deserializer::extract_first_byte(T & val, const std::span<const uint8_t> buffer) const noexcept
{
    val = buffer[byte_start];
    if(isSameByte())
        val = val >> first_byte_shift;
    val &= first_byte_mask;
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr void Deserializer::extract_intermediate_bytes(T & val, const std::span<const uint8_t> buffer) const noexcept
{
    for(size_t i=byte_start + 1; i<byte_end; i++)
        val = (val << 8) | buffer[i];
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr void Deserializer::extract_last_byte(T & val, const std::span<const uint8_t> buffer) const noexcept
{
    if(not isSameByte())
        val = (val << last_byte_shift) | (buffer[byte_end] >> (8 - last_byte_shift));
}

//-----------------------------------------------------------------------------
template<typename T>
constexpr void Deserializer::extend_sign(T & val) const noexcept
{
    val = static_cast<T>(val << sign_shift) >> sign_shift;
}

} // namespace bits::detail

#endif /* BITS_DETAIL_DESERIALIZER_H */