////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_BITS_FIELD_H
#define BITS_BITS_FIELD_H

#include <cstdint>
#include <climits>
#include <compare>

namespace bits {

//-----------------------------------------------------------------------------
//- Value wrapper class to handle bits field with a specified underlying type
//- Wrapper class to handle bits (de)serializable value
//-----------------------------------------------------------------------------
template<typename T, size_t HIGH, size_t LOW = HIGH>
class BitsField
{
    static_assert((sizeof(T) * CHAR_BIT) >= (HIGH - LOW + 1));

public:
    // Constructors and assignments
    constexpr inline BitsField(void) noexcept = default;
    constexpr inline explicit BitsField(const T & val) noexcept;
    constexpr inline explicit BitsField(T && val) noexcept;
    constexpr inline BitsField(const BitsField & val) noexcept = default;
    constexpr inline BitsField(BitsField && val) noexcept = default;
    constexpr inline BitsField & operator =(const T & rhs) noexcept;
    constexpr inline BitsField & operator =(T && rhs) noexcept;
    constexpr inline BitsField & operator =(const BitsField & rhs) noexcept = default;
    constexpr inline BitsField & operator =(BitsField && rhs) noexcept = default;

    // Relationnal operators
    constexpr auto operator <=>(const BitsField & rhs) const noexcept = default;

    // Logical operator
    constexpr bool operator !(void) const noexcept;

    // Cast operators
    explicit constexpr operator bool (void) const noexcept;

    // Accessors
    constexpr operator const T & (void) const noexcept;
    constexpr operator T & (void) noexcept;
    constexpr T & get(void) noexcept;
    constexpr const T & get(void) const noexcept;

protected:
    T value = {};
};



//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//- Constructors and assignments
//-----------------------------------------------------------------------------
template<typename T, size_t HIGH, size_t LOW>
constexpr inline BitsField<T, HIGH, LOW>::BitsField(const T & val) noexcept
: value(val)
{}

template<typename T, size_t HIGH, size_t LOW>
constexpr inline BitsField<T, HIGH, LOW>::BitsField(T && val) noexcept
: value(std::move(val))
{}

template<typename T, size_t HIGH, size_t LOW>
constexpr inline BitsField<T, HIGH, LOW> & BitsField<T, HIGH, LOW>::operator =(const T & rhs) noexcept
{
    value = rhs;
    return *this;
}

template<typename T, size_t HIGH, size_t LOW>
constexpr inline BitsField<T, HIGH, LOW> & BitsField<T, HIGH, LOW>::operator =(T && rhs) noexcept
{
    value = std::move(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
//- Logical operator
//-----------------------------------------------------------------------------
template<typename T, size_t HIGH, size_t LOW>
constexpr bool BitsField<T, HIGH, LOW>::operator !(void) const noexcept { return !value; }

//-----------------------------------------------------------------------------
//- Cast operators
//-----------------------------------------------------------------------------
template<typename T, size_t HIGH, size_t LOW>
constexpr BitsField<T, HIGH, LOW>::operator bool(void) const noexcept { return !!value; }

//-----------------------------------------------------------------------------
//- Accessors
//-----------------------------------------------------------------------------
template<typename T, size_t HIGH, size_t LOW>
constexpr BitsField<T, HIGH, LOW>::operator const T &(void) const noexcept { return value; }
template<typename T, size_t HIGH, size_t LOW>
constexpr BitsField<T, HIGH, LOW>::operator T &(void) noexcept { return value; }
template<typename T, size_t HIGH, size_t LOW>
constexpr const T & BitsField<T, HIGH, LOW>::get(void) const noexcept { return value; }
template<typename T, size_t HIGH, size_t LOW>
constexpr T & BitsField<T, HIGH, LOW>::get(void) noexcept { return value; }

} // namespace bits

#endif /* BITS_BITS_FIELD_H */