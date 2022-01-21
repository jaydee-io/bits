////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#ifndef BITS_HELPER_MACROS_H
#define BITS_HELPER_MACROS_H

#include <bits/BitsTraits.h>

//-----------------------------------------------------------------------------
//- Internal helper macros
//-----------------------------------------------------------------------------

// Namespaces
#define __BITS_EMPTY_NAMESPACE
#define __BITS_BEGIN_NAMESPACE(nameSpace)  namespace nameSpace {
#define __BITS_END_NAMESPACE(nameSpace)    }; // namespace nameSpace

// Enum traits : size()
#define __BITS_ENUM_DECLARE_TRAITS(nameSpace, name, ...) \
template<> struct bits::detail::IsBitEnum<nameSpace name> : std::true_type {}; \
template<> struct bits::EnumTraits<nameSpace name> { \
    inline static constexpr size_t size(void) { return __BITS_NUM_ARGS(__VA_ARGS__) / 2u; } \
};

// Arguments count
#define __BITS_NUM_ARGS(...) __BITS_NUM_ARGS2(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __BITS_NUM_ARGS2(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N17, N18, N19, N20, N21, N22, N23, N24, N25, N26, N27, N28, N29, N30, N31, N32, N33, N34, N35, N36, N37, N38, N39, N40, N41, N42, N43, N44, N45, N46, N47, N48, N49, N50, N51, N52, N53, N54, N55, N56, N57, N58, N59, N60, N61, N62, N63, N64, N, ...) N

// Recurse macros
#define __BITS_RECURSE_PAIR(func, ...)                          __BITS_DO2(__BITS_PAIR,   __BITS_NUM_ARGS(__VA_ARGS__))(func, __VA_ARGS__)
#define __BITS_RECURSE_SINGLE(func, sep, nameSpace, name, ...)  __BITS_DO2(__BITS_SINGLE, __BITS_NUM_ARGS(__VA_ARGS__))(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_DO2(func, N) __BITS_DO3(func, N)
#define __BITS_DO3(func, N) func ## _ ## N

// Recursively call a macro with a pair of arguments
#define __BITS_PAIR_2(  func, a, b)       func(a, b)
#define __BITS_PAIR_4(  func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_2(  func, __VA_ARGS__)
#define __BITS_PAIR_6(  func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_4(  func, __VA_ARGS__)
#define __BITS_PAIR_8(  func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_6(  func, __VA_ARGS__)
#define __BITS_PAIR_10( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_8(  func, __VA_ARGS__)
#define __BITS_PAIR_12( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_10( func, __VA_ARGS__)
#define __BITS_PAIR_14( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_12( func, __VA_ARGS__)
#define __BITS_PAIR_16( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_14( func, __VA_ARGS__)
#define __BITS_PAIR_18( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_16( func, __VA_ARGS__)
#define __BITS_PAIR_20( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_18( func, __VA_ARGS__)
#define __BITS_PAIR_22( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_20( func, __VA_ARGS__)
#define __BITS_PAIR_24( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_22( func, __VA_ARGS__)
#define __BITS_PAIR_26( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_24( func, __VA_ARGS__)
#define __BITS_PAIR_28( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_26( func, __VA_ARGS__)
#define __BITS_PAIR_30( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_28( func, __VA_ARGS__)
#define __BITS_PAIR_32( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_30( func, __VA_ARGS__)
#define __BITS_PAIR_34( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_32( func, __VA_ARGS__)
#define __BITS_PAIR_36( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_34( func, __VA_ARGS__)
#define __BITS_PAIR_38( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_36( func, __VA_ARGS__)
#define __BITS_PAIR_40( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_38( func, __VA_ARGS__)
#define __BITS_PAIR_42( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_40( func, __VA_ARGS__)
#define __BITS_PAIR_44( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_42( func, __VA_ARGS__)
#define __BITS_PAIR_46( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_44( func, __VA_ARGS__)
#define __BITS_PAIR_48( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_46( func, __VA_ARGS__)
#define __BITS_PAIR_50( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_48( func, __VA_ARGS__)
#define __BITS_PAIR_52( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_50( func, __VA_ARGS__)
#define __BITS_PAIR_54( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_52( func, __VA_ARGS__)
#define __BITS_PAIR_56( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_54( func, __VA_ARGS__)
#define __BITS_PAIR_58( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_56( func, __VA_ARGS__)
#define __BITS_PAIR_60( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_58( func, __VA_ARGS__)
#define __BITS_PAIR_62( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_60( func, __VA_ARGS__)
#define __BITS_PAIR_64( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_62( func, __VA_ARGS__)
#define __BITS_PAIR_66( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_64( func, __VA_ARGS__)
#define __BITS_PAIR_68( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_66( func, __VA_ARGS__)
#define __BITS_PAIR_70( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_68( func, __VA_ARGS__)
#define __BITS_PAIR_72( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_70( func, __VA_ARGS__)
#define __BITS_PAIR_74( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_72( func, __VA_ARGS__)
#define __BITS_PAIR_76( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_74( func, __VA_ARGS__)
#define __BITS_PAIR_78( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_76( func, __VA_ARGS__)
#define __BITS_PAIR_80( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_78( func, __VA_ARGS__)
#define __BITS_PAIR_82( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_80( func, __VA_ARGS__)
#define __BITS_PAIR_84( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_82( func, __VA_ARGS__)
#define __BITS_PAIR_86( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_84( func, __VA_ARGS__)
#define __BITS_PAIR_88( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_86( func, __VA_ARGS__)
#define __BITS_PAIR_90( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_88( func, __VA_ARGS__)
#define __BITS_PAIR_92( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_90( func, __VA_ARGS__)
#define __BITS_PAIR_94( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_92( func, __VA_ARGS__)
#define __BITS_PAIR_96( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_94( func, __VA_ARGS__)
#define __BITS_PAIR_98( func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_96( func, __VA_ARGS__)
#define __BITS_PAIR_100(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_98( func, __VA_ARGS__)
#define __BITS_PAIR_102(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_100(func, __VA_ARGS__)
#define __BITS_PAIR_104(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_102(func, __VA_ARGS__)
#define __BITS_PAIR_106(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_104(func, __VA_ARGS__)
#define __BITS_PAIR_108(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_106(func, __VA_ARGS__)
#define __BITS_PAIR_110(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_108(func, __VA_ARGS__)
#define __BITS_PAIR_112(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_110(func, __VA_ARGS__)
#define __BITS_PAIR_114(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_112(func, __VA_ARGS__)
#define __BITS_PAIR_116(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_114(func, __VA_ARGS__)
#define __BITS_PAIR_118(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_116(func, __VA_ARGS__)
#define __BITS_PAIR_120(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_118(func, __VA_ARGS__)
#define __BITS_PAIR_122(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_120(func, __VA_ARGS__)
#define __BITS_PAIR_124(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_122(func, __VA_ARGS__)
#define __BITS_PAIR_126(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_124(func, __VA_ARGS__)
#define __BITS_PAIR_128(func, a, b, ...)  __BITS_PAIR_2(func, a, b) , __BITS_PAIR_126(func, __VA_ARGS__)

// Recursively call a macro with a single argument (ignoring the second one)
#define __BITS_SINGLE_2(  func, sep, nameSpace, name, a, b)       func(nameSpace, name, a)
#define __BITS_SINGLE_4(  func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_2(  func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_6(  func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_4(  func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_8(  func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_6(  func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_10( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_8(  func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_12( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_10( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_14( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_12( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_16( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_14( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_18( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_16( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_20( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_18( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_22( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_20( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_24( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_22( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_26( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_24( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_28( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_26( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_30( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_28( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_32( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_30( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_34( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_32( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_36( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_34( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_38( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_36( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_40( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_38( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_42( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_40( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_44( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_42( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_46( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_44( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_48( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_46( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_50( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_48( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_52( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_50( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_54( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_52( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_56( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_54( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_58( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_56( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_60( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_58( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_62( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_60( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_64( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_62( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_66( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_64( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_68( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_66( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_70( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_68( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_72( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_70( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_74( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_72( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_76( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_74( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_78( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_76( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_80( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_78( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_82( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_80( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_84( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_82( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_86( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_84( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_88( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_86( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_90( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_88( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_92( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_90( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_94( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_92( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_96( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_94( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_98( func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_96( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_100(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_98( func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_102(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_100(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_104(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_102(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_106(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_104(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_108(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_106(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_110(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_108(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_112(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_110(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_114(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_112(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_116(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_114(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_118(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_116(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_120(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_118(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_122(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_120(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_124(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_122(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_126(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_124(func, sep, nameSpace, name, __VA_ARGS__)
#define __BITS_SINGLE_128(func, sep, nameSpace, name, a, b, ...)  __BITS_SINGLE_2(func, sep, nameSpace, name, a, b) sep __BITS_SINGLE_126(func, sep, nameSpace, name, __VA_ARGS__)

#endif /* BITS_HELPER_MACROS_H */
