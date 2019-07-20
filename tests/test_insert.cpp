#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/bits.h>

using ::testing::ElementsAre;

TEST(BitsInsertion, Unsigned_8bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint8_t>(0x03, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0x01, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0x7F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0xDC, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0x0D, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsInsertion, Unsigned_16bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint16_t>(0x0003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint16_t>(0x0001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint16_t>(0x007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint16_t>(0x3703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsInsertion, Unsigned_32bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint32_t>(0x00000003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x00000001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x0000007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x00003703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x34, 0x00));
}

TEST(BitsInsertion, Unsigned_8bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint8_t>(0x03, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint8_t>(0x01, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint8_t>(0x7F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint8_t>(0xDC, buffer, 21, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint8_t>(0x0D, buffer, 29, 22); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x37, 0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(BitsInsertion, Unsigned_16bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint16_t>(0x0003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x0001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x3703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(BitsInsertion, Unsigned_32bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint32_t>(0x00000003, buffer, 3,  0 ); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x00000001, buffer, 5,  4 ); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x0000007F, buffer, 13, 6 ); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x00003703, buffer, 27, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 53, 28); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x37, 0xFF));
}

TEST(BitsInsertion, Unsigned_8bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint8_t>(0x03, buffer, 37, 34); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0x01, buffer, 39, 38); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00));
    bits::insert<uint8_t>(0x7F, buffer, 47, 40); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0x00, 0x00));
    bits::insert<uint8_t>(0xDC, buffer, 55, 48); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x00));
    bits::insert<uint8_t>(0x0D, buffer, 63, 56); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_16bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint16_t>(0x0003, buffer, 39, 36); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00));
    bits::insert<uint16_t>(0x0001, buffer, 41, 40); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00));
    bits::insert<uint16_t>(0x007F, buffer, 49, 42); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xC0, 0x00));
    bits::insert<uint16_t>(0x3703, buffer, 63, 50); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03));
}

TEST(BitsInsertion, Unsigned_32bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    bits::insert<uint32_t>(0x00000003, buffer, 13, 10); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x00000001, buffer, 15, 14); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x0000007F, buffer, 23, 16); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x00003703, buffer, 37, 24); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0xDC, 0x0C, 0x00, 0x00, 0x00));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 63, 38); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_8bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint8_t>(0x03, buffer, 37, 34); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0XFF));
    bits::insert<uint8_t>(0x01, buffer, 39, 38); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0xFF, 0xFF, 0XFF));
    bits::insert<uint8_t>(0x7F, buffer, 47, 40); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xFF, 0XFF));
    bits::insert<uint8_t>(0xDC, buffer, 55, 48); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0XFF));
    bits::insert<uint8_t>(0x0D, buffer, 63, 56); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_16bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint16_t>(0x0003, buffer, 39, 36); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x0001, buffer, 41, 40); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x007F, buffer, 49, 42); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xFF, 0xFF));
    bits::insert<uint16_t>(0x3703, buffer, 63, 50); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xF7, 0x03));
}

TEST(BitsInsertion, Unsigned_32bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    bits::insert<uint32_t>(0x00000003, buffer, 13, 10); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x00000001, buffer, 15, 14); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x0000007F, buffer, 23, 16); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x00003703, buffer, 37, 24); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xDC, 0x0F, 0xFF, 0xFF, 0xFF));
    bits::insert<uint32_t>(0x017FDC0D, buffer, 63, 38); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D));
}
