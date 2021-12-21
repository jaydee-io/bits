////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_DETAIL_H
#define BITS_DETAIL_H

#include <stdint.h>
#include <cstdlib>

#include <bits/detail/infos.h>
#include <bits/detail/underlying_integral_type.h>

namespace bits::detail {

//-----------------------------------------------------------------------------
template<typename T>
void insert_last_byte(T & val, uint8_t * buffer, const SerializeInfos & infos)
{
    if(infos.byte_start != infos.byte_end)
    {
        uint8_t inserted_val = ((val << infos.first_byte_shift) & 0xFF);
        buffer[infos.byte_end] = buffer[infos.byte_end] | inserted_val;
        buffer[infos.byte_end] = buffer[infos.byte_end] & (inserted_val | ((1 << infos.first_byte_shift) - 1));
        val = val >> (8 - infos.first_byte_shift);
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void insert_intermediate_bytes(T & val, uint8_t * buffer, const SerializeInfos & infos)
{
    for(size_t i=infos.byte_end ? infos.byte_end - 1 : infos.byte_start; i>infos.byte_start; i--)
    {
        buffer[i] = val & 0xFF;
        val = val >> 8;
    }
}

//-----------------------------------------------------------------------------
template<typename T>
void insert_first_byte(T & val, uint8_t * buffer, const SerializeInfos & infos)
{
    if(infos.byte_start == infos.byte_end)
        val = (val << infos.first_byte_shift) & ~infos.first_byte_mask;
    buffer[infos.byte_start] = buffer[infos.byte_start] | val;
    buffer[infos.byte_start] = buffer[infos.byte_start] & (val | infos.first_byte_mask);
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_first_byte(T & val, const uint8_t * buffer, const DeserializeInfos & infos)
{
    val = buffer[infos.byte_start];
    if(infos.byte_start == infos.byte_end)
        val = val >> infos.first_byte_shift;
    val = val & infos.first_byte_mask;
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_intermediate_bytes(T & val, const uint8_t * buffer, const DeserializeInfos & infos)
{
    for(size_t i=infos.byte_start + 1; i<infos.byte_end; i++)
        val = (val << 8) | buffer[i];
}

//-----------------------------------------------------------------------------
template<typename T>
void extract_last_byte(T & val, const uint8_t * buffer, const DeserializeInfos & infos)
{
    if(infos.byte_start != infos.byte_end)
        val = (val << infos.last_byte_shift) | (buffer[infos.byte_end] >> (8 - infos.last_byte_shift));
}

//-----------------------------------------------------------------------------
template<typename T>
void extend_sign(T & val, const DeserializeInfos & infos)
{
    val = static_cast<T>(val << infos.sign_shift) >> infos.sign_shift;
}

//-----------------------------------------------------------------------------
template<typename T>
void insert(T val, uint8_t * buffer, const SerializeInfos & infos)
{
    auto rawVal = static_cast<underlying_integral_type_t<T>>(val);

    bits::detail::insert_last_byte         (rawVal, buffer, infos);
    bits::detail::insert_intermediate_bytes(rawVal, buffer, infos);
    bits::detail::insert_first_byte        (rawVal, buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, const DeserializeInfos & infos)
{
    underlying_integral_type_t<T> val = {};

    bits::detail::extract_first_byte        (val, buffer, infos);
    bits::detail::extract_intermediate_bytes(val, buffer, infos);
    bits::detail::extract_last_byte         (val, buffer, infos);

    if constexpr(std::is_signed_v<T>)
        bits::detail::extend_sign(val, infos);

    return static_cast<T>(val);
}

} // namespace bits::detail

#endif /* BITS_DETAIL_H */