////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstddef>

#include <bits/bits_insertion.h>

using ::testing::ElementsAreArray;

#define make_array(a, b, c, d, e, f, g, h) { std::byte(a), std::byte(b), std::byte(c), std::byte(d), std::byte(e), std::byte(f), std::byte(g), std::byte(h) }

TEST(BitsInsertion_CArray, Unsigned_8bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint8_t>(0x03, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0x01, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0x7F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0xDC, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0x0D, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, Unsigned_16bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint16_t>(0x0003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t>(0x0001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t>(0x007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t>(0x3703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, Unsigned_32bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint32_t>(0x00000003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x00000001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x0000007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x00003703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x34, 0x00)));
}

TEST(BitsInsertion_CArray, Unsigned_8bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint8_t>(0x03, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t>(0x01, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t>(0x7F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t>(0xDC, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t>(0x0D, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x37, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, Unsigned_16bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint16_t>(0x0003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x0001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x3703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, Unsigned_32bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint32_t>(0x00000003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x00000001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x0000007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x00003703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x37, 0xFF)));
}

TEST(BitsInsertion_CArray, Unsigned_8bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint8_t>(0x03, buffer, 37, 34); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0x01, buffer, 39, 38); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t>(0x7F, buffer, 47, 40); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0x00, 0x00)));
    bits::insert<uint8_t>(0xDC, buffer, 55, 48); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x00)));
    bits::insert<uint8_t>(0x0D, buffer, 63, 56); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, Unsigned_16bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint16_t>(0x0003, buffer, 39, 36); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t>(0x0001, buffer, 41, 40); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00)));
    bits::insert<uint16_t>(0x007F, buffer, 49, 42); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xC0, 0x00)));
    bits::insert<uint16_t>(0x3703, buffer, 63, 50); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03)));
}

TEST(BitsInsertion_CArray, Unsigned_32bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint32_t>(0x00000003, buffer, 13, 10); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x00000001, buffer, 15, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x0000007F, buffer, 23, 16); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x00003703, buffer, 37, 24); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0C, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 63, 38); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, Unsigned_8bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint8_t>(0x03, buffer, 37, 34); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0XFF)));
    bits::insert<uint8_t>(0x01, buffer, 39, 38); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0xFF, 0xFF, 0XFF)));
    bits::insert<uint8_t>(0x7F, buffer, 47, 40); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xFF, 0XFF)));
    bits::insert<uint8_t>(0xDC, buffer, 55, 48); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0XFF)));
    bits::insert<uint8_t>(0x0D, buffer, 63, 56); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, Unsigned_16bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint16_t>(0x0003, buffer, 39, 36); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x0001, buffer, 41, 40); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x007F, buffer, 49, 42); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xFF, 0xFF)));
    bits::insert<uint16_t>(0x3703, buffer, 63, 50); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xF7, 0x03)));
}

TEST(BitsInsertion_CArray, Unsigned_32bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint32_t>(0x00000003, buffer, 13, 10); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x00000001, buffer, 15, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x0000007F, buffer, 23, 16); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x00003703, buffer, 37, 24); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xDC, 0x0F, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 63, 38); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, Signed_8bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int8_t>(  -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t>(  -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t>(  63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t>(-113, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t>( 115, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCC, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, Signed_16bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int16_t>(   -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t>(   -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t>(   63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t>(-7204, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xC0, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, Signed_32bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int32_t>(     -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t>(     -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t>(     63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t>(  -7204, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xC0, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t>(-573553, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xFF, 0xDC, 0xFE, 0x3C, 0x00)));
}

TEST(BitsInsertion_CArray, Signed_8bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int8_t>(  -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t>(  -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t>(  63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t>(-113, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t>( 115, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, Signed_16bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int16_t>(   -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t>(   -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t>(   63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t>(-7204, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, Signed_32bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int32_t>(     -3, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t>(     -1, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t>(     63, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t>(  -7204, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t>(-573553, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xFF, 0xDC, 0xFE, 0x3F, 0xFF)));
}

//-----------------------------------------------------------------------------
//- Tests for templated bits position
//-----------------------------------------------------------------------------

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_8bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint8_t, 3,  0 >(0x03, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 5,  4 >(0x01, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 13, 6 >(0x7F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 21, 14>(0xDC, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 29, 22>(0x0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_16bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint16_t, 3,  0 >(0x0003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t, 5,  4 >(0x0001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t, 13, 6 >(0x007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t, 27, 14>(0x3703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_32bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint32_t, 3,  0 >(0x00000003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 5,  4 >(0x00000001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 13, 6 >(0x0000007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 27, 14>(0x00003703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 53, 28>(0x017FDC0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x34, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_8bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint8_t, 3,  0 >(0x03, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t, 5,  4 >(0x01, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t, 13, 6 >(0x7F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t, 21, 14>(0xDC, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint8_t, 29, 22>(0x0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x37, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_16bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint16_t, 3,  0 >(0x0003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t, 5,  4 >(0x0001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t, 13, 6 >(0x007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t, 27, 14>(0x3703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_32bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint32_t, 3,  0 >(0x00000003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 5,  4 >(0x00000001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 13, 6 >(0x0000007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 27, 14>(0x00003703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 53, 28>(0x017FDC0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x37, 0xFF)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_8bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint8_t, 37, 34>(0x03, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 39, 38>(0x01, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00)));
    bits::insert<uint8_t, 47, 40>(0x7F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0x00, 0x00)));
    bits::insert<uint8_t, 55, 48>(0xDC, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x00)));
    bits::insert<uint8_t, 63, 56>(0x0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_16bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint16_t, 39, 36>(0x0003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00)));
    bits::insert<uint16_t, 41, 40>(0x0001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00)));
    bits::insert<uint16_t, 49, 42>(0x007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xC0, 0x00)));
    bits::insert<uint16_t, 63, 50>(0x3703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_32bits_with_offset_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<uint32_t, 13, 10>(0x00000003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 15, 14>(0x00000001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 23, 16>(0x0000007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 37, 24>(0x00003703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0C, 0x00, 0x00, 0x00)));
    bits::insert<uint32_t, 63, 38>(0x017FDC0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_8bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint8_t, 37, 34>(0x03, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0XFF)));
    bits::insert<uint8_t, 39, 38>(0x01, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0xFF, 0xFF, 0XFF)));
    bits::insert<uint8_t, 47, 40>(0x7F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xFF, 0XFF)));
    bits::insert<uint8_t, 55, 48>(0xDC, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0XFF)));
    bits::insert<uint8_t, 63, 56>(0x0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_16bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint16_t, 39, 36>(0x0003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint16_t, 41, 40>(0x0001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFF, 0xFF)));
    bits::insert<uint16_t, 49, 42>(0x007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xFF, 0xFF)));
    bits::insert<uint16_t, 63, 50>(0x3703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xF7, 0x03)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Unsigned_32bits_with_offset_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<uint32_t, 13, 10>(0x00000003, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 15, 14>(0x00000001, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 23, 16>(0x0000007F, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 37, 24>(0x00003703, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xDC, 0x0F, 0xFF, 0xFF, 0xFF)));
    bits::insert<uint32_t, 63, 38>(0x017FDC0D, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xFF, 0xCD, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_8bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int8_t, 3,  0 >(  -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t, 5,  4 >(  -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t, 13, 6 >(  63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t, 21, 14>(-113, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int8_t, 29, 22>( 115, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCC, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_16bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int16_t, 3,  0 >(   -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t, 5,  4 >(   -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t, 13, 6 >(   63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int16_t, 27, 14>(-7204, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xC0, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_32bits_empty_buffer)
{
    std::byte buffer [] = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert<int32_t, 3,  0 >(     -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t, 5,  4 >(     -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t, 13, 6 >(     63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t, 27, 14>(  -7204, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xC0, 0x00, 0x00, 0x00, 0x00)));
    bits::insert<int32_t, 53, 28>(-573553, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xFF, 0xDC, 0xFE, 0x3C, 0x00)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_8bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int8_t, 3,  0 >(  -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t, 5,  4 >(  -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t, 13, 6 >(  63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t, 21, 14>(-113, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int8_t, 29, 22>( 115, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_16bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int16_t, 3,  0 >(   -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t, 5,  4 >(   -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t, 13, 6 >(   63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int16_t, 27, 14>(-7204, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
}

TEST(BitsInsertion_CArray, TemplatedPosition_Signed_32bits_no_empty_buffer)
{
    std::byte buffer [] = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    bits::insert<int32_t, 3,  0 >(     -3, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t, 5,  4 >(     -1, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t, 13, 6 >(     63, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t, 27, 14>(  -7204, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF)));
    bits::insert<int32_t, 53, 28>(-573553, buffer); ASSERT_THAT(buffer, ElementsAreArray(make_array(0xDC, 0xFE, 0x3D, 0xFF, 0xDC, 0xFE, 0x3F, 0xFF)));
}
