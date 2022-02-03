////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_ENUM_H
#define BITS_ENUM_H

#include <bits/detail/helper_macros.h>
#include <string_view>

//-----------------------------------------------------------------------------
//- Helper macros to declare strongly type enums.
//- This also add the 'to_string()' free function.
//-
//- BITS_DECLARE_ENUM[...]
//- (
//-     Name,            => declare an enum <Name> and a flags type named Flags<Name>
//-     Type,            => optional underlying enum type
//-     VAL_1, 0x32      => declare an enum value named 'VAL_1' with value '0x32'
//-     VAL_4, (4 * 256) => declare an enum value named 'VAL_4' with value 1024 (4 * 256)
//-     ...
//- )
//-----------------------------------------------------------------------------
#define BITS_DECLARE_ENUM(name, ...) \
    __BITS_ENUM_DECLARE_ENUM(name, , __VA_ARGS__) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_ENUM_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_ENUM_WITH_TYPE(name, rawType, ...) \
    __BITS_ENUM_DECLARE_ENUM(name, : rawType, __VA_ARGS__) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_ENUM_DECLARE_TO_STRING(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(__BITS_EMPTY_NAMESPACE, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_ENUM_WITH_NAMESPACE(nameSpace, name, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_ENUM_DECLARE_ENUM(name, , __VA_ARGS__) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_ENUM_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)

#define BITS_DECLARE_ENUM_WITH_TYPE_AND_NAMESPACE(nameSpace, name, rawType, ...) \
    __BITS_BEGIN_NAMESPACE(nameSpace) \
    __BITS_ENUM_DECLARE_ENUM(name, : rawType, __VA_ARGS__) \
    __BITS_END_NAMESPACE(nameSpace) \
    __BITS_BEGIN_NAMESPACE(bits) \
    __BITS_ENUM_DECLARE_TO_STRING(nameSpace::, name, __VA_ARGS__) \
    __BITS_ENUM_DECLARE_TRAITS(nameSpace::, name, __VA_ARGS__) \
    __BITS_END_NAMESPACE(bits)


//-----------------------------------------------------------------------------
//- Internal macros
//-----------------------------------------------------------------------------
#define __BITS_ENUM_DECLARE_ENUM(name, rawTypeSpec, ...) \
enum class name rawTypeSpec { \
    __BITS_ENUM_DECLARE_ALL_ENUM_VALUES(__VA_ARGS__) \
};
#define __BITS_ENUM_DECLARE_ALL_ENUM_VALUES(...)    __BITS_RECURSE_PAIR(__BITS_ENUM_DECLARE_ENUM_VALUE, __VA_ARGS__)
#define __BITS_ENUM_DECLARE_ENUM_VALUE(name, val)   name = val

// Enum to string_view free standing function
#define __BITS_ENUM_DECLARE_TO_STRING(nameSpace, name, ...) \
inline std::string_view to_string(nameSpace name value) { \
    switch(value) { \
        __BITS_ENUM_DECLARE_ALL_TO_STRING_VALUES(nameSpace, name, __VA_ARGS__) \
    } \
    return "<invalid>"; \
}
#define __BITS_ENUM_DECLARE_ALL_TO_STRING_VALUES(nameSpace, name, ...)  __BITS_RECURSE_SINGLE(__BITS_ENUM_DECLARE_TO_STRING_VALUE, , nameSpace, name, __VA_ARGS__)
#define __BITS_ENUM_DECLARE_TO_STRING_VALUE(nameSpace, name, val)       case nameSpace name::val : return #val;

#endif // BITS_ENUM_H
