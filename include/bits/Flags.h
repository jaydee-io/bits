////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
// Adapted from Khronos Group Vulkan HPP Flags
// See https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/vulkan/vulkan.hpp
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_FLAGS_H
#define BITS_FLAGS_H

#include <bits/detail/helper_macros.h>
#include <bits/BitsTraits.h>
#include <type_traits>
#include <string>
#include <compare>

namespace bits {

//-----------------------------------------------------------------------------
//- Traits to identify Flags types and declare a constant with all flags set
//- Specialized, via the BITS_DECLARE_FLAGS[...] maros, for each Flag type
//-----------------------------------------------------------------------------
template<typename EnumType>
struct FlagsTraits
{
    static constexpr const auto ALL_FLAGS = 0;
};

//-----------------------------------------------------------------------------
//- Utility class to handle flags
//- By 'flags', we means :
//-     - strongly typed enum values
//-     - each value setting one bit at a time
//-     - bitwise composable (and, or, xor, ...)
//-     - stricly ordered
//-----------------------------------------------------------------------------
template<typename EnumType>
class Flags
{
public:
    using MaskType = typename std::underlying_type<EnumType>::type;

    // Constructors
    constexpr Flags(void) noexcept = default;
    constexpr Flags(EnumType val) noexcept;
    constexpr Flags(const Flags & val) noexcept = default;
    constexpr explicit Flags(MaskType val) noexcept;

    // Relationnal operators
    constexpr auto operator <=>(const Flags & rhs) const noexcept = default;

    // Logical operator
    constexpr bool operator !(void) const noexcept;

    // Bitwise operators
    constexpr Flags operator &(const Flags & rhs) const noexcept;
    constexpr Flags operator |(const Flags & rhs) const noexcept;
    constexpr Flags operator ^(const Flags & rhs) const noexcept;
    constexpr Flags operator ~(void)              const noexcept;

    // Assignment operators
    constexpr Flags & operator  =(const Flags & rhs) noexcept = default;
    constexpr Flags & operator &=(const Flags & rhs) noexcept;
    constexpr Flags & operator |=(const Flags & rhs) noexcept;
    constexpr Flags & operator ^=(const Flags & rhs) noexcept;

    // Cast operators
    explicit constexpr operator bool    (void) const noexcept;
    explicit constexpr operator MaskType(void) const noexcept;

    // Basic flag setting and checking
    constexpr void set     (const Flags & val) noexcept;
    constexpr void unset   (const Flags & val) noexcept;
    constexpr void toggle  (const Flags & val) noexcept;
    constexpr bool isSet   (const Flags & val) const noexcept;
    constexpr bool isNotSet(const Flags & val) const noexcept;

private:
    MaskType mask = 0;
};

//-----------------------------------------------------------------------------
//- Bitwise operators enum/flags
//-----------------------------------------------------------------------------
template<typename EnumType>
inline constexpr Flags<EnumType> operator &(EnumType lhs, const Flags<EnumType> & rhs) noexcept;
template<typename EnumType>
inline constexpr Flags<EnumType> operator |(EnumType lhs, const Flags<EnumType> & rhs) noexcept;
template<typename EnumType>
inline constexpr Flags<EnumType> operator ^(EnumType lhs, const Flags<EnumType> & rhs) noexcept;

} // namespace bits

//-----------------------------------------------------------------------------
//- Bitwise operators enum/enum
//-----------------------------------------------------------------------------
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator &(EnumType lhs, EnumType rhs) noexcept;
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator |(EnumType lhs, EnumType rhs) noexcept;
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator ^(EnumType lhs, EnumType rhs) noexcept;
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator ~(EnumType lhs) noexcept;


//-----------------------------------------------------------------------------
//- Helper macros to declare flags.
//- This also add the 'to_string()' free function.
//-
//- BITS_DECLARE_FLAGS[...]
//- (
//-     NameSpace   => declare enum and flags types into namespace
//-     Name,       => declare an enum <Name> and a flags type named Flags<Name>
//-     Type,       => optional underlying enum type
//-     VAL_1, 2    => declare an enum value named 'VAL_1' with third bit set : 0b100
//-     BIT_4, 4    => declare an enum value named 'BIT_4' with fifth bit set : 0b10000
//-     ...
//- )
//-----------------------------------------------------------------------------
#define BITS_DECLARE_FLAGS(name, ...) \
    __BITS_FLAGS_DECLARE_ENUM(name, , __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_FLAGS_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_FLAGS_WITH_TYPE(name, rawType, ...) \
    __BITS_FLAGS_DECLARE_ENUM(name, : rawType, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_FLAGS_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_FLAGS_WITH_NAMESPACE(nameSpace, name, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_ENUM(name, , __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_FLAGS_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_FLAGS_WITH_TYPE_AND_NAMESPACE(nameSpace, name, rawType, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_ENUM(name, : rawType, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_FLAGS_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)





//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

namespace bits {

//-----------------------------------------------------------------------------
//- Constructors
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr Flags<EnumType>::Flags(EnumType val) noexcept
: mask(static_cast<MaskType>(val))
{}

template<typename EnumType>
constexpr Flags<EnumType>::Flags(MaskType val) noexcept
: mask(val)
{}

//-----------------------------------------------------------------------------
//- Logical operator
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr bool Flags<EnumType>::operator !(void) const noexcept { return !mask; }

//-----------------------------------------------------------------------------
//- Bitwise operators
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr Flags<EnumType> Flags<EnumType>::operator &(const Flags & rhs) const noexcept { return Flags(mask & rhs.mask); }
template<typename EnumType>
constexpr Flags<EnumType> Flags<EnumType>::operator |(const Flags & rhs) const noexcept { return Flags(mask | rhs.mask); }
template<typename EnumType>
constexpr Flags<EnumType> Flags<EnumType>::operator ^(const Flags & rhs) const noexcept { return Flags(mask ^ rhs.mask); }
template<typename EnumType>
constexpr Flags<EnumType> Flags<EnumType>::operator ~(void)              const noexcept { return Flags(mask ^ FlagsTraits<EnumType>::ALL_FLAGS); }

//-----------------------------------------------------------------------------
//- Assignment operators
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr Flags<EnumType> & Flags<EnumType>::operator &=(const Flags & rhs) noexcept { mask &= rhs.mask; return *this; }
template<typename EnumType>
constexpr Flags<EnumType> & Flags<EnumType>::operator |=(const Flags & rhs) noexcept { mask |= rhs.mask; return *this; }
template<typename EnumType>
constexpr Flags<EnumType> & Flags<EnumType>::operator ^=(const Flags & rhs) noexcept { mask ^= rhs.mask; return *this; }

//-----------------------------------------------------------------------------
//- Cast operators
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr Flags<EnumType>::operator bool    (void) const noexcept { return !!mask; }
template<typename EnumType>
constexpr Flags<EnumType>::operator MaskType(void) const noexcept { return   mask; }

//-----------------------------------------------------------------------------
//- Basic flag setting and checking
//-----------------------------------------------------------------------------
template<typename EnumType>
constexpr void Flags<EnumType>::set     (const Flags & val) noexcept { mask |=  val.mask; }
template<typename EnumType>
constexpr void Flags<EnumType>::unset   (const Flags & val) noexcept { mask &= ~val.mask; }
template<typename EnumType>
constexpr void Flags<EnumType>::toggle  (const Flags & val) noexcept { mask ^=  val.mask; }
template<typename EnumType>
constexpr bool Flags<EnumType>::isSet   (const Flags & val) const noexcept { return static_cast<bool>( (*this) & val); }
template<typename EnumType>
constexpr bool Flags<EnumType>::isNotSet(const Flags & val) const noexcept { return static_cast<bool>(~(*this) & val); }

//-----------------------------------------------------------------------------
//- Bitwise operators enum/flags
//-----------------------------------------------------------------------------
template<typename EnumType>
inline constexpr Flags<EnumType> operator &(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs & lhs; }
template<typename EnumType>
inline constexpr Flags<EnumType> operator |(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs | lhs; }
template<typename EnumType>
inline constexpr Flags<EnumType> operator ^(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs ^ lhs; }

} // namespace bits

//-----------------------------------------------------------------------------
//- Bitwise operators enum/enum
//-----------------------------------------------------------------------------
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator &(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) & rhs; }
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator |(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) | rhs; }
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator ^(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) ^ rhs; }
template<typename EnumType>
requires(bits::is_flags_bits_enum_v<EnumType>)
inline constexpr bits::Flags<EnumType> operator ~(EnumType lhs)               noexcept { return ~bits::Flags<EnumType>(lhs); }

//-----------------------------------------------------------------------------
//- Internal macros
//-----------------------------------------------------------------------------
// Flags type name
#define __BITS_FLAGS_NAME(name) Flags ## name

// Enum type
#define __BITS_FLAGS_DECLARE_ENUM(name, rawTypeSpec, ...) \
enum class name rawTypeSpec { \
    __BITS_FLAGS_DECLARE_ALL_ENUM_VALUES(__VA_ARGS__) \
};
#define __BITS_FLAGS_DECLARE_ALL_ENUM_VALUES(...)    __BITS_RECURSE_PAIR(__BITS_FLAGS_DECLARE_ENUM_VALUE, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_ENUM_VALUE(name, val)   name = (1 << (val))

// Flags type using
#define __BITS_FLAGS_DECLARE_USING(name) using __BITS_FLAGS_NAME(name) = bits::Flags<name>;

// Flags traits
#define __BITS_FLAGS_DECLARE_TRAITS(nameSpace, name, ...) \
template<> struct detail::IsFlagsBitsEnum<nameSpace name> : std::true_type {}; \
template<> struct detail::IsFlagsBits<nameSpace __BITS_FLAGS_NAME(name)> : std::true_type {}; \
template<> \
struct FlagsTraits<nameSpace name> : std::true_type { \
    static constexpr const auto ALL_FLAGS = \
        __BITS_FLAGS_DECLARE_ALL_TRAIT_VALUES(nameSpace, name, __VA_ARGS__) \
    ; \
};
#define __BITS_FLAGS_DECLARE_ALL_TRAIT_VALUES(nameSpace, name, ...)    __BITS_RECURSE_SINGLE(__BITS_FLAGS_DECLARE_TRAIT_VALUE, |, nameSpace, name, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_TRAIT_VALUE(nameSpace, name, val)         nameSpace __BITS_FLAGS_NAME(name)::MaskType(nameSpace name::val)

// Flags to string free standing function
#define __BITS_FLAGS_DECLARE_TO_STRING(nameSpace, name, ...) \
inline std::string to_string(nameSpace __BITS_FLAGS_NAME(name) value) { \
    if(not value) \
        return "{}"; \
    std::string result; \
    __BITS_FLAGS_DECLARE_ALL_TO_STRING_VALUES(nameSpace, name, __VA_ARGS__) \
    return "{ " + result.substr(0, result.size() - 3) + " }"; \
}
#define __BITS_FLAGS_DECLARE_ALL_TO_STRING_VALUES(nameSpace, name, ...) __BITS_RECURSE_SINGLE(__BITS_FLAGS_DECLARE_TO_STRING_VALUE, , nameSpace, name, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_TO_STRING_VALUE(nameSpace, name, val)      if(value & nameSpace name::val) result += #val " | ";

#endif /* BITS_FLAGS_H */
