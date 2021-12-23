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
#include <type_traits>
#include <string>
#if __cplusplus >= 202002L
#include <compare>
#endif // __cplusplus >= 202002L

namespace bits {

//-----------------------------------------------------------------------------
//- Traits to identify Flags types and declare a constant with all flags set
//-----------------------------------------------------------------------------
template<typename EnumType>
struct FlagsTraits : std::false_type {
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
class Flags {
public:
    using MaskType = typename std::underlying_type<EnumType>::type;

    // Constructors
    constexpr Flags(void)                        noexcept : mask(0) {}
    constexpr Flags(EnumType val)                noexcept : mask(static_cast<MaskType>(val)) {}
    constexpr Flags(const Flags<EnumType> & val) noexcept : mask(val.mask) {}
    constexpr explicit Flags(MaskType val)       noexcept : mask(val) {}

    // Relationnal operators
#if __cplusplus >= 202002L
    constexpr auto operator <=>(const Flags<EnumType> & rhs) const noexcept = default;
#else // __cplusplus < 202002L
    constexpr bool operator  < (const Flags<EnumType> & rhs) const noexcept { return mask <  rhs.mask; }
    constexpr bool operator  <=(const Flags<EnumType> & rhs) const noexcept { return mask <= rhs.mask; }
    constexpr bool operator  > (const Flags<EnumType> & rhs) const noexcept { return mask >  rhs.mask; }
    constexpr bool operator  >=(const Flags<EnumType> & rhs) const noexcept { return mask >= rhs.mask; }
    constexpr bool operator  ==(const Flags<EnumType> & rhs) const noexcept { return mask == rhs.mask; }
    constexpr bool operator  !=(const Flags<EnumType> & rhs) const noexcept { return mask != rhs.mask; }
#endif // __cplusplus >= 202002L

    // Logical operator
    constexpr bool operator !(void) const noexcept { return !mask; }

    // Bitwise operators
    constexpr Flags<EnumType> operator &(const Flags<EnumType> & rhs) const noexcept { return Flags<EnumType>(mask & rhs.mask); }
    constexpr Flags<EnumType> operator |(const Flags<EnumType> & rhs) const noexcept { return Flags<EnumType>(mask | rhs.mask); }
    constexpr Flags<EnumType> operator ^(const Flags<EnumType> & rhs) const noexcept { return Flags<EnumType>(mask ^ rhs.mask); }
    constexpr Flags<EnumType> operator ~(void)                        const noexcept { return Flags<EnumType>(mask ^ FlagsTraits<EnumType>::ALL_FLAGS); }

    // Assignment operators
    constexpr Flags<EnumType> & operator  =(const Flags<EnumType> & rhs) noexcept { mask  = rhs.mask; return *this; }
    constexpr Flags<EnumType> & operator &=(const Flags<EnumType> & rhs) noexcept { mask &= rhs.mask; return *this; }
    constexpr Flags<EnumType> & operator |=(const Flags<EnumType> & rhs) noexcept { mask |= rhs.mask; return *this; }
    constexpr Flags<EnumType> & operator ^=(const Flags<EnumType> & rhs) noexcept { mask ^= rhs.mask; return *this; }

    // Cast operators
    explicit constexpr operator bool    (void) const noexcept { return !!mask; }
    explicit constexpr operator MaskType(void) const noexcept { return   mask; }

    // Basic flag setting and checking
    constexpr void set     (const Flags<EnumType> & val) { mask |=  val.mask; }
    constexpr void unset   (const Flags<EnumType> & val) { mask &= ~val.mask; }
    constexpr void toggle  (const Flags<EnumType> & val) { mask ^=  val.mask; }
    constexpr bool isSet   (const Flags<EnumType> & val) { return static_cast<bool>( (*this) & val); }
    constexpr bool isNotSet(const Flags<EnumType> & val) { return static_cast<bool>(~(*this) & val); }

private:
    MaskType mask;
};

//-----------------------------------------------------------------------------
//- Traits to check if a type is a flags enum type
//-----------------------------------------------------------------------------
template<typename T>
inline constexpr bool is_flags_bits_v = FlagsTraits<T>::value;

//-----------------------------------------------------------------------------
//- Relationnal operators
//-----------------------------------------------------------------------------
#if __cplusplus < 202002L
template<typename EnumType>
inline constexpr bool operator < (EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs >  lhs; }
template<typename EnumType>
inline constexpr bool operator <=(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs >= lhs; }
template<typename EnumType>
inline constexpr bool operator > (EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs <  lhs; }
template<typename EnumType>
inline constexpr bool operator >=(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs <= lhs; }
template<typename EnumType>
inline constexpr bool operator ==(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs == lhs; }
template<typename EnumType>
inline constexpr bool operator !=(EnumType lhs, const Flags<EnumType> & rhs) noexcept { return rhs != lhs; }
#endif // __cplusplus < 202002L

} // namespace bits

//-----------------------------------------------------------------------------
//- Bitwise operators enum/flags
//-----------------------------------------------------------------------------
template<typename EnumType>
inline constexpr bits::Flags<EnumType> operator &(EnumType lhs, const bits::Flags<EnumType> & rhs) noexcept { return rhs & lhs; }
template<typename EnumType>
inline constexpr bits::Flags<EnumType> operator |(EnumType lhs, const bits::Flags<EnumType> & rhs) noexcept { return rhs | lhs; }
template<typename EnumType>
inline constexpr bits::Flags<EnumType> operator ^(EnumType lhs, const bits::Flags<EnumType> & rhs) noexcept { return rhs ^ lhs; }

//-----------------------------------------------------------------------------
//- Bitwise operators enum/enum
//-----------------------------------------------------------------------------
template<typename EnumType, std::enable_if_t<bits::is_flags_bits_v<EnumType>, bool> = true>
inline constexpr bits::Flags<EnumType> operator &(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) & rhs; }
template<typename EnumType, std::enable_if_t<bits::is_flags_bits_v<EnumType>, bool> = true>
inline constexpr bits::Flags<EnumType> operator |(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) | rhs; }
template<typename EnumType, std::enable_if_t<bits::is_flags_bits_v<EnumType>, bool> = true>
inline constexpr bits::Flags<EnumType> operator ^(EnumType lhs, EnumType rhs) noexcept { return  bits::Flags<EnumType>(lhs) ^ rhs; }
template<typename EnumType, std::enable_if_t<bits::is_flags_bits_v<EnumType>, bool> = true>
inline constexpr bits::Flags<EnumType> operator ~(EnumType lhs)               noexcept { return ~bits::Flags<EnumType>(lhs); }

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
    __BITS_FLAGS_DECLARE_ENUM(name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_FLAGS_DECLARE_TO_STRING(name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAIT(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__)

#define BITS_DECLARE_FLAGS_WITH_TYPE(name, rawType, ...) \
    __BITS_FLAGS_DECLARE_ENUM_WITH_TYPE(name, rawType, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_FLAGS_DECLARE_TO_STRING(name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_TRAIT(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__)

#define BITS_DECLARE_FLAGS_WITH_NAMESPACE(nameSpace, name, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_ENUM(name, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_FLAGS_DECLARE_TO_STRING(name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_TRAIT(nameSpace::, name, __VA_ARGS__)

#define BITS_DECLARE_FLAGS_WITH_TYPE_AND_NAMESPACE(nameSpace, name, rawType, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_ENUM_WITH_TYPE(name, rawType, __VA_ARGS__) \
    __BITS_FLAGS_DECLARE_USING(name) \
    __BITS_FLAGS_DECLARE_TO_STRING(name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_FLAGS_DECLARE_TRAIT(nameSpace::, name, __VA_ARGS__)





//-----------------------------------------------------------------------------
//-
//- Implementation
//-
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//- Internal macros
//-----------------------------------------------------------------------------
// Flags type name
#define __BITS_FLAGS_NAME(name) Flags ## name

// Enum type
#define __BITS_FLAGS_DECLARE_ENUM(name, ...) \
enum class name { \
    __BITS_FLAGS_DECLARE_ALL_ENUM_VALUES(__VA_ARGS__) \
};
#define __BITS_FLAGS_DECLARE_ENUM_WITH_TYPE(name, rawType, ...) \
enum class name : rawType { \
    __BITS_FLAGS_DECLARE_ALL_ENUM_VALUES(__VA_ARGS__) \
};
#define __BITS_FLAGS_DECLARE_ALL_ENUM_VALUES(...)    __BITS_RECURSE_PAIR(__BITS_FLAGS_DECLARE_ENUM_VALUE, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_ENUM_VALUE(name, val)   name = (1 << (val))

// Flags type using
#define __BITS_FLAGS_DECLARE_USING(name) using __BITS_FLAGS_NAME(name) = bits::Flags<name>;

// Flags traits
#define __BITS_FLAGS_DECLARE_TRAIT(nameSpace, name, ...) \
namespace bits { \
template<> \
struct FlagsTraits<nameSpace name> : std::true_type{ \
    static constexpr const auto ALL_FLAGS = \
        __BITS_FLAGS_DECLARE_ALL_TRAIT_VALUES(nameSpace, name, __VA_ARGS__) \
    ; \
}; \
}; // namespace bits
#define __BITS_FLAGS_DECLARE_ALL_TRAIT_VALUES(nameSpace, name, ...)    __BITS_RECURSE_SINGLE(__BITS_FLAGS_DECLARE_TRAIT_VALUE, |, nameSpace, name, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_TRAIT_VALUE(nameSpace, name, val)         nameSpace __BITS_FLAGS_NAME(name)::MaskType(nameSpace name::val)

// Flags to string free standing function
#define __BITS_FLAGS_DECLARE_TO_STRING(name, ...) \
inline std::string to_string(__BITS_FLAGS_NAME(name) value) { \
    if(not value) \
        return "{}"; \
    std::string result; \
    __BITS_FLAGS_DECLARE_ALL_TO_STRING_VALUES(name, __VA_ARGS__) \
    return "{ " + result.substr(0, result.size() - 3) + " }"; \
}
#define __BITS_FLAGS_DECLARE_ALL_TO_STRING_VALUES(name, ...)            __BITS_RECURSE_SINGLE(__BITS_FLAGS_DECLARE_TO_STRING_VALUE, , __BITS_EMPTY_NAMESPACE, name, __VA_ARGS__)
#define __BITS_FLAGS_DECLARE_TO_STRING_VALUE(nameSpace, name, val)      if(value & name::val) result += #val " | ";

#endif /* BITS_FLAGS_H */
