#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/bits.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

TEST(BitsExtraction, NbBitsDeserialized)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(deserializer.nbBitsDeserialized(), 0);
    deserializer.extract<uint8_t>(4); ASSERT_EQ(deserializer.nbBitsDeserialized(), 4);
    deserializer.extract<uint8_t>(2); ASSERT_EQ(deserializer.nbBitsDeserialized(), 6);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsDeserialized(), 14);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsDeserialized(), 22);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsDeserialized(), 30);
}

TEST(BitsExtraction, Unsigned_8bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(deserializer.extract<uint8_t>(4), 0x03);
    ASSERT_EQ(deserializer.extract<uint8_t>(2), 0x01);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0x7F);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0xDC);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0x0D);
}

TEST(BitsExtraction, Unsigned_16bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(deserializer.extract<uint16_t>(4),  0x0003);
    ASSERT_EQ(deserializer.extract<uint16_t>(2),  0x0001);
    ASSERT_EQ(deserializer.extract<uint16_t>(8),  0x007F);
    ASSERT_EQ(deserializer.extract<uint16_t>(14), 0x3703);
}

TEST(BitsExtraction, Unsigned_32bits)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(deserializer.extract<uint32_t>(4),  0x00000003);
    ASSERT_EQ(deserializer.extract<uint32_t>(2),  0x00000001);
    ASSERT_EQ(deserializer.extract<uint32_t>(8),  0x0000007F);
    ASSERT_EQ(deserializer.extract<uint32_t>(14), 0x00003703);
    ASSERT_EQ(deserializer.extract<uint32_t>(26), 0x017FDC0D);
}

TEST(BitsExtraction, Unsigned_8bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D };
    size_t nbBitsToDeserialize = 30;
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToDeserialize);

    ASSERT_EQ(deserializer.extract<uint8_t>(4), 0x03);
    ASSERT_EQ(deserializer.extract<uint8_t>(2), 0x01);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0x7F);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0xDC);
    ASSERT_EQ(deserializer.extract<uint8_t>(8), 0x0D);
}

TEST(BitsExtraction, Unsigned_16bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03 };
    size_t nbBitsToDeserialize = 28;
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToDeserialize);

    ASSERT_EQ(deserializer.extract<uint16_t>(4),  0x0003);
    ASSERT_EQ(deserializer.extract<uint16_t>(2),  0x0001);
    ASSERT_EQ(deserializer.extract<uint16_t>(8),  0x007F);
    ASSERT_EQ(deserializer.extract<uint16_t>(14), 0x3703);
}

TEST(BitsExtraction, Unsigned_32bits_with_offset)
{
    uint8_t buffer[] = { 0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D };
    size_t nbBitsToDeserialize = 54;
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToDeserialize);

    ASSERT_EQ(deserializer.extract<uint32_t>(4),  0x00000003);
    ASSERT_EQ(deserializer.extract<uint32_t>(2),  0x00000001);
    ASSERT_EQ(deserializer.extract<uint32_t>(8),  0x0000007F);
    ASSERT_EQ(deserializer.extract<uint32_t>(14), 0x00003703);
    ASSERT_EQ(deserializer.extract<uint32_t>(26), 0x017FDC0D);
}