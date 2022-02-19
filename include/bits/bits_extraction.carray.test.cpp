////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstddef>
#include <span>
#include <array>
#include <vector>
#include <list>

#include <bits/bits_extraction.h>

using ::testing::ElementsAreArray;

#define make_array(a, b, c, d, e, f, g, h) { std::byte(a), std::byte(b), std::byte(c), std::byte(d), std::byte(e), std::byte(f), std::byte(g), std::byte(h) }

template<typename T = std::byte, typename... Ts>
constexpr std::array<const T, sizeof...(Ts)> make_cpp_array(Ts && ... args) noexcept
{
    return { T(std::forward<Ts>(args))... };
}

TEST(BitsExtraction_CArray, Unsigned_8bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ(bits::extract<uint8_t>(buffer, 3,  0 ), 0x03);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 5,  4 ), 0x01);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 13, 6 ), 0x7F);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 21, 14), 0xDC);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 29, 22), 0x0D);
}

TEST(BitsExtraction_CArray, Unsigned_16bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ(bits::extract<uint16_t>(buffer, 3,  0 ), 0x0003);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 5,  4 ), 0x0001);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 13, 6 ), 0x007F);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 27, 14), 0x3703);
}

TEST(BitsExtraction_CArray, Unsigned_32bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ(bits::extract<uint32_t>(buffer, 3,  0 ), 0x00000003);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 5,  4 ), 0x00000001);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 13, 6 ), 0x0000007F);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 27, 14), 0x00003703);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 53, 28), 0x017FDC0D);
}

TEST(BitsExtraction_CArray, Unsigned_8bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D);

    ASSERT_EQ(bits::extract<uint8_t>(buffer, 37, 34), 0x03);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 39, 38), 0x01);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 47, 40), 0x7F);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 55, 48), 0xDC);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 63, 56), 0x0D);
}

TEST(BitsExtraction_CArray, Unsigned_16bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03);

    ASSERT_EQ(bits::extract<uint16_t>(buffer, 39, 36), 0x0003);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 41, 40), 0x0001);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 49, 42), 0x007F);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 63, 50), 0x3703);
}

TEST(BitsExtraction_CArray, Unsigned_32bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D);

    ASSERT_EQ(bits::extract<uint32_t>(buffer, 13, 10), 0x00000003);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 15, 14), 0x00000001);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 23, 16), 0x0000007F);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 37, 24), 0x00003703);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 63, 38), 0x017FDC0D);
}

TEST(BitsExtraction_CArray, Signed_8bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ(bits::extract<int8_t>(buffer, 3,  0 ),   -3);
    ASSERT_EQ(bits::extract<int8_t>(buffer, 5,  4 ),   -1);
    ASSERT_EQ(bits::extract<int8_t>(buffer, 13, 6 ),   63);
    ASSERT_EQ(bits::extract<int8_t>(buffer, 21, 14), -113);
    ASSERT_EQ(bits::extract<int8_t>(buffer, 29, 22),  115);
}

TEST(BitsExtraction_CArray, Signed_16bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ(bits::extract<int16_t>(buffer, 3,  0 ),    -3);
    ASSERT_EQ(bits::extract<int16_t>(buffer, 5,  4 ),    -1);
    ASSERT_EQ(bits::extract<int16_t>(buffer, 13, 6 ),    63);
    ASSERT_EQ(bits::extract<int16_t>(buffer, 27, 14), -7204);
}

TEST(BitsExtraction_CArray, Signed_32bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ(bits::extract<int32_t>(buffer, 3,  0 ),      -3);
    ASSERT_EQ(bits::extract<int32_t>(buffer, 5,  4 ),      -1);
    ASSERT_EQ(bits::extract<int32_t>(buffer, 13, 6 ),      63);
    ASSERT_EQ(bits::extract<int32_t>(buffer, 27, 14),   -7204);
    ASSERT_EQ(bits::extract<int32_t>(buffer, 53, 28), -573553);
}

TEST(BitsExtraction_CArray, Ranges)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[3] = {};
    bits::extract(buffer, c_array, 27, 4);
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::array<uint8_t, 3> std_array = {};
    bits::extract(buffer, std_array, 27, 4);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std_array = {};
    std::span<uint8_t, 3> std_span(std_array);
    bits::extract(buffer, std_span, 27, 4);
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::vector<uint8_t> std_vector(3, 0);
    bits::extract(buffer, std_vector, 27, 4);
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::list<uint8_t> std_list(3, 0);
    bits::extract(buffer, std_list, 27, 4);
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Extract array APIs with returned value
    std_array = {};
    std_array = bits::extract<std::array<uint8_t, 3>>(buffer, 27, 4);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Function cannot return array type
    // c_array = bits::extract<uint8_t[3]>(buffer, 27, 4);
    // ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Function cannot return std::span type
    // std_span = bits::extract<std::span<uint8_t, 3>>(buffer, 27, 4);
    // ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));
}

TEST(BitsExtraction_CArray, Ranges_Subranges)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[6] = {};
    bits::extract(buffer, std::ranges::subrange(std::next(std::begin(c_array)), std::next(std::begin(c_array), 4)), 27, 4);
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::array<uint8_t, 6> std_array = {};
    bits::extract(buffer, std::ranges::subrange(std::next(std::begin(std_array)), std::next(std::begin(std_array), 4)), 27, 4);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std_array = {};
    std::span<uint8_t, 6> std_span(std_array);
    bits::extract(buffer, std::ranges::subrange(std::next(std::begin(std_span)), std::next(std::begin(std_span), 4)), 27, 4);
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::vector<uint8_t> std_vector(6, 0);
    bits::extract(buffer, std::ranges::subrange(std::next(std::begin(std_vector)), std::next(std::begin(std_vector), 4)), 27, 4);
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::list<uint8_t> std_list(6, 0);
    bits::extract(buffer, std::ranges::subrange(std::next(std::begin(std_list)), std::next(std::begin(std_list), 4)), 27, 4);
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
}

TEST(BitsExtraction_CArray, Ranges_Small_Values)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[3] = {};
    bits::extract(buffer, c_array, 15, 4, 4);
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::array<uint8_t, 3> std_array = {};
    bits::extract(buffer, std_array, 15, 4, 4);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std_array = {};
    std::span<uint8_t, 3> std_span(std_array);
    bits::extract(buffer, std_span, 15, 4, 4);
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::vector<uint8_t> std_vector(3, 0);
    bits::extract(buffer, std_vector, 15, 4, 4);
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::list<uint8_t> std_list(3, 0);
    bits::extract(buffer, std_list, 15, 4, 4);
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Extract array APIs with returned value
    std_array = {};
    std_array = bits::extract<std::array<uint8_t, 3>>(buffer, 15, 4, 4);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Function cannot return array type
    // c_array = bits::extract<uint8_t[3]>(buffer, 15, 4, 4);
    // ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Function cannot return std::span type
    // std_span = bits::extract<std::span<uint8_t, 3>>(buffer, 15, 4, 4);
    // ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));
}

//-----------------------------------------------------------------------------
//- Tests for templated bits position
//-----------------------------------------------------------------------------

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_8bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ((bits::extract<3,  0,  uint8_t>(buffer)), 0x03);
    ASSERT_EQ((bits::extract<5,  4,  uint8_t>(buffer)), 0x01);
    ASSERT_EQ((bits::extract<13, 6,  uint8_t>(buffer)), 0x7F);
    ASSERT_EQ((bits::extract<21, 14, uint8_t>(buffer)), 0xDC);
    ASSERT_EQ((bits::extract<29, 22, uint8_t>(buffer)), 0x0D);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_16bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ((bits::extract<3,  0,  uint16_t>(buffer)), 0x0003);
    ASSERT_EQ((bits::extract<5,  4,  uint16_t>(buffer)), 0x0001);
    ASSERT_EQ((bits::extract<13, 6,  uint16_t>(buffer)), 0x007F);
    ASSERT_EQ((bits::extract<27, 14, uint16_t>(buffer)), 0x3703);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_32bits)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    ASSERT_EQ((bits::extract<3,  0,  uint32_t>(buffer)), 0x00000003);
    ASSERT_EQ((bits::extract<5,  4,  uint32_t>(buffer)), 0x00000001);
    ASSERT_EQ((bits::extract<13, 6,  uint32_t>(buffer)), 0x0000007F);
    ASSERT_EQ((bits::extract<27, 14, uint32_t>(buffer)), 0x00003703);
    ASSERT_EQ((bits::extract<53, 28, uint32_t>(buffer)), 0x017FDC0D);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_8bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D);

    ASSERT_EQ((bits::extract<37, 34, uint8_t>(buffer)), 0x03);
    ASSERT_EQ((bits::extract<39, 38, uint8_t>(buffer)), 0x01);
    ASSERT_EQ((bits::extract<47, 40, uint8_t>(buffer)), 0x7F);
    ASSERT_EQ((bits::extract<55, 48, uint8_t>(buffer)), 0xDC);
    ASSERT_EQ((bits::extract<63, 56, uint8_t>(buffer)), 0x0D);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_16bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03);

    ASSERT_EQ((bits::extract<39, 36, uint16_t>(buffer)), 0x0003);
    ASSERT_EQ((bits::extract<41, 40, uint16_t>(buffer)), 0x0001);
    ASSERT_EQ((bits::extract<49, 42, uint16_t>(buffer)), 0x007F);
    ASSERT_EQ((bits::extract<63, 50, uint16_t>(buffer)), 0x3703);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Unsigned_32bits_with_offset)
{
    const std::byte buffer [] = make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D);

    ASSERT_EQ((bits::extract<13, 10, uint32_t>(buffer)), 0x00000003);
    ASSERT_EQ((bits::extract<15, 14, uint32_t>(buffer)), 0x00000001);
    ASSERT_EQ((bits::extract<23, 16, uint32_t>(buffer)), 0x0000007F);
    ASSERT_EQ((bits::extract<37, 24, uint32_t>(buffer)), 0x00003703);
    ASSERT_EQ((bits::extract<63, 38, uint32_t>(buffer)), 0x017FDC0D);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Signed_8bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ((bits::extract<3,  0,  int8_t>(buffer)),   -3);
    ASSERT_EQ((bits::extract<5,  4,  int8_t>(buffer)),   -1);
    ASSERT_EQ((bits::extract<13, 6,  int8_t>(buffer)),   63);
    ASSERT_EQ((bits::extract<21, 14, int8_t>(buffer)), -113);
    ASSERT_EQ((bits::extract<29, 22, int8_t>(buffer)),  115);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Signed_16bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ((bits::extract<3,  0,  int16_t>(buffer)),    -3);
    ASSERT_EQ((bits::extract<5,  4,  int16_t>(buffer)),    -1);
    ASSERT_EQ((bits::extract<13, 6,  int16_t>(buffer)),    63);
    ASSERT_EQ((bits::extract<27, 14, int16_t>(buffer)), -7204);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Signed_32bits)
{
    const std::byte buffer [] = make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xDC, 0xFE, 0x3D, 0xC7);

    ASSERT_EQ((bits::extract<3,  0,  int32_t>(buffer)),      -3);
    ASSERT_EQ((bits::extract<5,  4,  int32_t>(buffer)),      -1);
    ASSERT_EQ((bits::extract<13, 6,  int32_t>(buffer)),      63);
    ASSERT_EQ((bits::extract<27, 14, int32_t>(buffer)),   -7204);
    ASSERT_EQ((bits::extract<53, 28, int32_t>(buffer)), -573553);
}

TEST(BitsExtraction_CArray, TemplatedPosition_Ranges)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[3] = {};
    bits::extract<27, 4>(buffer, c_array);
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::array<uint8_t, 3> std_array = {};
    bits::extract<27, 4>(buffer, std_array);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std_array = {};
    std::span<uint8_t, 3> std_span(std_array);
    bits::extract<27, 4>(buffer, std_span);
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::vector<uint8_t> std_vector(3, 0);
    bits::extract<27, 4>(buffer, std_vector);
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    std::list<uint8_t> std_list(3, 0);
    bits::extract<27, 4>(buffer, std_list);
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Extract array APIs with returned value
    std_array = bits::extract<27, 4, decltype(std_array)>(buffer);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Function cannot return array type
    // c_array = bits::extract<27, 4, uint8_t[3]>(buffer);
    // ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));

    // Function cannot return std::span type
    // std_span = bits::extract<27, 4, std::span<uint8_t, 3>>(buffer);
    // ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x5F, 0xF7, 0x03)));
}

TEST(BitsExtraction_CArray, TemplatedPosition_Ranges_Subranges)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[6] = {};
    bits::extract<27, 4>(buffer, std::ranges::subrange(std::next(std::begin(c_array)), std::next(std::begin(c_array), 4)));
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::array<uint8_t, 6> std_array = {};
    bits::extract<27, 4>(buffer, std::ranges::subrange(std::next(std::begin(std_array)), std::next(std::begin(std_array), 4)));
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std_array = {};
    std::span<uint8_t, 6> std_span(std_array);
    bits::extract<27, 4>(buffer, std::ranges::subrange(std::next(std::begin(std_span)), std::next(std::begin(std_span), 4)));
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::vector<uint8_t> std_vector(6, 0);
    bits::extract<27, 4>(buffer, std::ranges::subrange(std::next(std::begin(std_vector)), std::next(std::begin(std_vector), 4)));
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));

    std::list<uint8_t> std_list(6, 0);
    bits::extract<27, 4>(buffer, std::ranges::subrange(std::next(std::begin(std_list)), std::next(std::begin(std_list), 4)));
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
}

TEST(BitsExtraction_CArray, TemplatedPosition_Ranges_Small_Values)
{
    const std::byte buffer [] = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    // Extract array APIs with output value
    uint8_t c_array[3] = {};
    bits::extract<15, 4, 4>(buffer, c_array);
    ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::array<uint8_t, 3> std_array = {};
    bits::extract<15, 4, 4>(buffer, std_array);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std_array = {};
    std::span<uint8_t, 3> std_span(std_array);
    bits::extract<15, 4, 4>(buffer, std_span);
    ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::vector<uint8_t> std_vector(3, 0);
    bits::extract<15, 4, 4>(buffer, std_vector);
    ASSERT_THAT(std_vector, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    std::list<uint8_t> std_list(3, 0);
    bits::extract<15, 4, 4>(buffer, std_list);
    ASSERT_THAT(std_list, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Extract array APIs with returned value
    std_array = bits::extract<15, 4, decltype(std_array), 4>(buffer);
    ASSERT_THAT(std_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Function cannot return array type
    // c_array = bits::extract<15, 4, uint8_t[3], 4>(buffer);
    // ASSERT_THAT(c_array, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));

    // Function cannot return std::span type
    // std_span = bits::extract<15, 4, std::span<uint8_t, 3>, 4>(buffer);
    // ASSERT_THAT(std_span, ElementsAreArray(make_cpp_array<uint8_t>(0x05, 0x0F, 0x0F)));
}
