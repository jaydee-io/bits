#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/bits.h>

using ::testing::ElementsAre;

TEST(BitsExtraction, Unsigned_8bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ(bits::extract<uint8_t>(buffer, 3,  0 ), 0x03);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 5,  4 ), 0x01);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 13, 6 ), 0x7F);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 21, 14), 0xDC);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 29, 22), 0x0D);
}

TEST(BitsExtraction, Unsigned_16bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ(bits::extract<uint16_t>(buffer, 3,  0 ), 0x0003);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 5,  4 ), 0x0001);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 13, 6 ), 0x007F);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 27, 14), 0x3703);
}

TEST(BitsExtraction, Unsigned_32bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ(bits::extract<uint32_t>(buffer, 3,  0 ), 0x00000003);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 5,  4 ), 0x00000001);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 13, 6 ), 0x0000007F);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 27, 14), 0x00003703);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 53, 28), 0x017FDC0D);
}

TEST(BitsExtraction, Unsigned_8bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D };

    ASSERT_EQ(bits::extract<uint8_t>(buffer, 37, 34), 0x03);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 39, 38), 0x01);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 47, 40), 0x7F);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 55, 48), 0xDC);
    ASSERT_EQ(bits::extract<uint8_t>(buffer, 63, 56), 0x0D);
}

TEST(BitsExtraction, Unsigned_16bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03 };

    ASSERT_EQ(bits::extract<uint16_t>(buffer, 39, 36), 0x0003);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 41, 40), 0x0001);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 49, 42), 0x007F);
    ASSERT_EQ(bits::extract<uint16_t>(buffer, 63, 50), 0x3703);
}

TEST(BitsExtraction, Unsigned_32bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D };

    ASSERT_EQ(bits::extract<uint32_t>(buffer, 13, 10), 0x00000003);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 15, 14), 0x00000001);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 23, 16), 0x0000007F);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 37, 24), 0x00003703);
    ASSERT_EQ(bits::extract<uint32_t>(buffer, 63, 38), 0x017FDC0D);
}

//-----------------------------------------------------------------------------
//- Tests for templated bits position
//-----------------------------------------------------------------------------

TEST(BitsExtraction, TemplatedPosition_Unsigned_8bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ((bits::extract<uint8_t, 3,  0 >(buffer)), 0x03);
    ASSERT_EQ((bits::extract<uint8_t, 5,  4 >(buffer)), 0x01);
    ASSERT_EQ((bits::extract<uint8_t, 13, 6 >(buffer)), 0x7F);
    ASSERT_EQ((bits::extract<uint8_t, 21, 14>(buffer)), 0xDC);
    ASSERT_EQ((bits::extract<uint8_t, 29, 22>(buffer)), 0x0D);
}

TEST(BitsExtraction, TemplatedPosition_Unsigned_16bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ((bits::extract<uint16_t, 3,  0 >(buffer)), 0x0003);
    ASSERT_EQ((bits::extract<uint16_t, 5,  4 >(buffer)), 0x0001);
    ASSERT_EQ((bits::extract<uint16_t, 13, 6 >(buffer)), 0x007F);
    ASSERT_EQ((bits::extract<uint16_t, 27, 14>(buffer)), 0x3703);
}

TEST(BitsExtraction, TemplatedPosition_Unsigned_32bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };

    ASSERT_EQ((bits::extract<uint32_t, 3,  0 >(buffer)), 0x00000003);
    ASSERT_EQ((bits::extract<uint32_t, 5,  4 >(buffer)), 0x00000001);
    ASSERT_EQ((bits::extract<uint32_t, 13, 6 >(buffer)), 0x0000007F);
    ASSERT_EQ((bits::extract<uint32_t, 27, 14>(buffer)), 0x00003703);
    ASSERT_EQ((bits::extract<uint32_t, 53, 28>(buffer)), 0x017FDC0D);
}

TEST(BitsExtraction, TemplatedPosition_Unsigned_8bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D };

    ASSERT_EQ((bits::extract<uint8_t, 37, 34>(buffer)), 0x03);
    ASSERT_EQ((bits::extract<uint8_t, 39, 38>(buffer)), 0x01);
    ASSERT_EQ((bits::extract<uint8_t, 47, 40>(buffer)), 0x7F);
    ASSERT_EQ((bits::extract<uint8_t, 55, 48>(buffer)), 0xDC);
    ASSERT_EQ((bits::extract<uint8_t, 63, 56>(buffer)), 0x0D);
}

TEST(BitsExtraction, TemplatedPosition_Unsigned_16bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03 };

    ASSERT_EQ((bits::extract<uint16_t, 39, 36>(buffer)), 0x0003);
    ASSERT_EQ((bits::extract<uint16_t, 41, 40>(buffer)), 0x0001);
    ASSERT_EQ((bits::extract<uint16_t, 49, 42>(buffer)), 0x007F);
    ASSERT_EQ((bits::extract<uint16_t, 63, 50>(buffer)), 0x3703);
}

TEST(BitsExtraction, TemplatedPosition_Unsigned_32bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D };

    ASSERT_EQ((bits::extract<uint32_t, 13, 10>(buffer)), 0x00000003);
    ASSERT_EQ((bits::extract<uint32_t, 15, 14>(buffer)), 0x00000001);
    ASSERT_EQ((bits::extract<uint32_t, 23, 16>(buffer)), 0x0000007F);
    ASSERT_EQ((bits::extract<uint32_t, 37, 24>(buffer)), 0x00003703);
    ASSERT_EQ((bits::extract<uint32_t, 63, 38>(buffer)), 0x017FDC0D);
}