////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_EXTRACTION_H
#define BITS_BITS_EXTRACTION_H

#include <cstddef>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <iterator>
#include <ranges>

#include <bits/detail/Deserializer.h>
#include <bits/detail/Traits.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Extract to output parameter with runtime bits range
//-----------------------------------------------------------------------------
template<typename T>
requires(not detail::output_range<T>)
constexpr void extract(const std::span<const std::byte> buffer, T & val, size_t high, size_t low);
template<detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last, size_t high, size_t low, size_t nbBitsByElement = sizeof(std::iter_value_t<O>) * CHAR_BIT);
template<detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

//-----------------------------------------------------------------------------
//- Extract to output parameter with compile time bits range
//-----------------------------------------------------------------------------
template<size_t high, size_t low, typename T>
requires(not detail::output_range<T>)
constexpr void extract(const std::span<const std::byte> buffer, T & val);
template<size_t high, size_t low, size_t nbBitsByElement, detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last);
template<size_t high, size_t low, detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last);
template<size_t high, size_t low, size_t nbBitsByElement, detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r);
template<size_t high, size_t low, detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r);

//-----------------------------------------------------------------------------
//- Extract to return parameter with runtime bits range
//-----------------------------------------------------------------------------
template<typename T>
requires(not detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low);
template<typename T>
requires(detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low, size_t nbBitsByElement = sizeof(std::ranges::range_value_t<T>) * CHAR_BIT);

//-----------------------------------------------------------------------------
//- Extract to output parameter with compile time bits range
//-----------------------------------------------------------------------------
template<size_t high, size_t low, typename T>
requires(not detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer);
template<size_t high, size_t low, typename T, size_t nbBitsByElement = sizeof(std::ranges::range_value_t<T>) * CHAR_BIT>
requires(detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer);




//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//- Detail helper to implement array extraction with compile time bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace detail {

template<size_t high, size_t low, size_t nbBitsByElement, detail::output_iterator O, std::size_t... I>
constexpr void extract(const std::span<const std::byte> buffer, O first, std::index_sequence<I...>)
{
    ((bits::extract<(I + 1) * nbBitsByElement + low - 1, I * nbBitsByElement + low>(buffer, *std::next(first, I))), ...);
}

} // namespace detail

//-----------------------------------------------------------------------------
//- Extract to output parameter with runtime bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
requires(not detail::output_range<T>)
constexpr void extract(const std::span<const std::byte> buffer, T & val, size_t high, size_t low)
{
    assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    const detail::Deserializer deserializer(sizeof(T) * CHAR_BIT, high, low);

    deserializer.extract(buffer, val);
}

//-----------------------------------------------------------------------------
template<detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last, [[maybe_unused]] size_t high, size_t low, size_t nbBitsByElement)
{
    assert((sizeof(std::iter_value_t<O>) * CHAR_BIT) >= nbBitsByElement);
    assert((std::distance(first, last) * sizeof(std::iter_value_t<O>) * CHAR_BIT) >= (high - low + 1));

    while(first != last)
    {
        extract(buffer, *first, low + nbBitsByElement - 1, low);
        first++;
        low += nbBitsByElement;
    }

    assert(high == (low - 1));
}

//-----------------------------------------------------------------------------
template<detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement)
{
    extract(buffer, std::ranges::begin(r), std::ranges::end(r), high, low, nbBitsByElement);
}

//-----------------------------------------------------------------------------
//- Extract to output parameter with compile time bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<size_t high, size_t low, typename T>
requires(not detail::output_range<T>)
constexpr void extract(const std::span<const std::byte> buffer, T & val)
{
    assert((buffer.size() * CHAR_BIT) >= (high - low + 1));
    static_assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    constexpr detail::Deserializer deserializer(sizeof(T) * CHAR_BIT, high, low);

    deserializer.extract(buffer, val);
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, size_t nbBitsByElement, detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, [[maybe_unused]] S last)
{
    constexpr auto nElems = (high - low + 1) / nbBitsByElement;
    static_assert((sizeof(std::iter_value_t<O>) * CHAR_BIT) >= nbBitsByElement);
    assert(static_cast<size_t>(std::distance(first, last)) >= nElems);

    detail::extract<high, low, nbBitsByElement>(buffer, first, std::make_index_sequence<nElems>());
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last)
{
    extract<high, low, sizeof(std::iter_value_t<O>) * CHAR_BIT>(buffer, first, last);
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, size_t nbBitsByElement, detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r)
{
    extract<high, low, nbBitsByElement>(buffer, std::ranges::begin(r), std::ranges::end(r));
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r)
{
    extract<high, low>(buffer, std::ranges::begin(r), std::ranges::end(r));
}

//-----------------------------------------------------------------------------
//- Extract to return parameter with runtime bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
requires(not detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low)
{
    static_assert(not detail::is_std_span_v<T>, "extract() cannot return std::span type");

    T val;
    extract(buffer, val, high, low);
    return val;
}

//-----------------------------------------------------------------------------
template<typename T>
requires(detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low, size_t nbBitsByElement)
{
    T val;
    extract(buffer, val, high, low, nbBitsByElement);
    return val;
}

//-----------------------------------------------------------------------------
//- Extract to output parameter with compile time bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<size_t high, size_t low, typename T>
requires(not detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer)
{
    static_assert(not detail::is_std_span_v<T>, "extract() cannot return std::span type");

    T val;
    extract<high, low>(buffer, val);
    return val;
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, typename T, size_t nbBitsByElement>
requires(detail::is_std_array_v<T>)
constexpr T extract(const std::span<const std::byte> buffer)
{
    T val;
    extract<high, low, nbBitsByElement>(buffer, val);
    return val;
}

} // namespace bits

#endif /* BITS_BITS_EXTRACTION_H */