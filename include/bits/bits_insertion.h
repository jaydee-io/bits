////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_INSERTION_H
#define BITS_BITS_INSERTION_H

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <span>
#include <iterator>
#include <ranges>

#include <bits/detail/Serializer.h>
#include <bits/detail/Traits.h>

namespace bits {

//-----------------------------------------------------------------------------
//- Free standing functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//- Insert value with runtime bits range
//-----------------------------------------------------------------------------
template<detail::input_basic_type T>
constexpr void insert(const std::span<std::byte> buffer, T val, size_t high, size_t low);
template<std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last, size_t high, size_t low, size_t nbBitsByElement = sizeof(std::iter_value_t<I>) * CHAR_BIT);
template<std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

//-----------------------------------------------------------------------------
//- Insert value with compile time bits range
//-----------------------------------------------------------------------------
template<size_t high, size_t low, detail::input_basic_type T>
constexpr void insert(const std::span<std::byte> buffer, T val);
template<size_t high, size_t low, size_t nbBitsByElement, std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last);
template<size_t high, size_t low, std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last);
template<size_t high, size_t low, size_t nbBitsByElement, std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r);
template<size_t high, size_t low, std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r);





//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//- Detail helper to implement array insertion with compile time bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace detail {

template<size_t high, size_t low, size_t nbBitsByElement, std::input_iterator It, std::size_t... I>
constexpr void insert(const std::span<std::byte> buffer, It first, std::index_sequence<I...>)
{
    (bits::insert<(I + 1) * nbBitsByElement + low - 1, I * nbBitsByElement + low>(buffer, *std::next(first, I)), ...);
}

} // namespace detail

//-----------------------------------------------------------------------------
//- Insert value with runtime bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<detail::input_basic_type T>
constexpr void insert(const std::span<std::byte> buffer, T val, size_t high, size_t low)
{
    assert((buffer.size() * CHAR_BIT) >= (high - low + 1));
    assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    const detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

//-----------------------------------------------------------------------------
template<std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, [[maybe_unused]] S last, [[maybe_unused]] size_t high, size_t low, size_t nbBitsByElement)
{
    assert((sizeof(std::iter_value_t<I>) * CHAR_BIT) >= nbBitsByElement);
    assert((std::distance(first, last) * sizeof(std::iter_value_t<I>) * CHAR_BIT) >= (high - low + 1));

    while(first != last)
    {
        insert(buffer, *first, low + nbBitsByElement - 1, low);
        first++;
        low += nbBitsByElement;
    }

    assert(high == (low - 1));
}

//-----------------------------------------------------------------------------
template<std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement)
{
    insert(buffer, std::ranges::begin(r), std::ranges::end(r), high, low, nbBitsByElement);
}

//-----------------------------------------------------------------------------
//- Insert value with compile time bits range
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<size_t high, size_t low, detail::input_basic_type T>
constexpr void insert(const std::span<std::byte> buffer, T val)
{
    assert((buffer.size() * CHAR_BIT) >= (high - low + 1));
    static_assert((sizeof(T) * CHAR_BIT) >= (high - low + 1));

    constexpr detail::Serializer serializer(high, low);

    serializer.insert(val, buffer);
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, size_t nbBitsByElement, std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, [[maybe_unused]] S last)
{
    constexpr auto nElems = (high - low + 1) / nbBitsByElement;
    static_assert((sizeof(std::iter_value_t<I>) * CHAR_BIT) >= nbBitsByElement);
    assert(static_cast<size_t>(std::distance(first, last)) >= nElems);

    detail::insert<high, low, nbBitsByElement>(buffer, first, std::make_index_sequence<nElems>());
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last)
{
    insert<high, low, sizeof(std::iter_value_t<I>) * CHAR_BIT>(buffer, first, last);
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, size_t nbBitsByElement, std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r)
{
    insert<high, low, nbBitsByElement>(buffer, std::ranges::begin(r), std::ranges::end(r));
}

//-----------------------------------------------------------------------------
template<size_t high, size_t low, std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r)
{
    insert<high, low>(buffer, std::ranges::begin(r), std::ranges::end(r));
}

} // namespace bits

#endif /* BITS_BITS_INSERTION_H */