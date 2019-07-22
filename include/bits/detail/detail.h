#ifndef BITS_DETAIL_H
#define BITS_DETAIL_H

#include <stdint.h>
#include <cstdlib>

#include "bits/detail/infos.h"

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
        val = val << infos.first_byte_shift;
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
void insert(T val, uint8_t * buffer, const SerializeInfos & infos)
{
    detail::insert_last_byte         (val, buffer, infos);
    detail::insert_intermediate_bytes(val, buffer, infos);
    detail::insert_first_byte        (val, buffer, infos);
}

//-----------------------------------------------------------------------------
template<typename T>
T extract(const uint8_t * buffer, const DeserializeInfos & infos)
{
    T val = 0;

    detail::extract_first_byte        (val, buffer, infos);
    detail::extract_intermediate_bytes(val, buffer, infos);
    detail::extract_last_byte         (val, buffer, infos);

    return val;
}

} // namespace bits::detail

#endif /* BITS_DETAIL_H */