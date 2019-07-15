#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <bits/bits.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

TEST(BitsInsertion, NbBitsSerialized)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(serializer.nbBitsSerialized(), 0);
    serializer.insert<uint8_t>(0x0, 4); ASSERT_EQ(serializer.nbBitsSerialized(), 4);
    serializer.insert<uint8_t>(0x0, 2); ASSERT_EQ(serializer.nbBitsSerialized(), 6);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsSerialized(), 14);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsSerialized(), 22);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsSerialized(), 30);
}

TEST(BitsInsertion, Unsigned_8bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint8_t>(0x03, 4); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0x01, 2); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0x7F, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0xDC, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0x0D, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsInsertion, Unsigned_16bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint16_t>(0x0003,  4); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint16_t>(0x0001,  2); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint16_t>(0x007F,  8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint16_t>(0x3703, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsInsertion, Unsigned_32bits_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint32_t>(0x00000003,  4); ASSERT_THAT(buffer, ElementsAre(0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x00000001,  2); ASSERT_THAT(buffer, ElementsAre(0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x0000007F,  8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x00003703, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x017FDC0D, 26); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x34, 0x00));
}

TEST(BitsInsertion, Unsigned_8bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint8_t>(0x03, 4); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint8_t>(0x01, 2); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint8_t>(0x7F, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint8_t>(0xDC, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint8_t>(0x0D, 8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x37, 0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(BitsInsertion, Unsigned_16bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint16_t>(0x0003,  4); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x0001,  2); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x007F,  8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x3703, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(BitsInsertion, Unsigned_32bits_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer.insert<uint32_t>(0x00000003,  4); ASSERT_THAT(buffer, ElementsAre(0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x00000001,  2); ASSERT_THAT(buffer, ElementsAre(0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x0000007F,  8); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x00003703, 14); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x017FDC0D, 26); ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x37, 0xFF));
}

TEST(BitsInsertion, Unsigned_8bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    size_t nbBitsToSerialize = 30;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint8_t>(0x03, 4); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0x01, 2); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00));
    serializer.insert<uint8_t>(0x7F, 8); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0x00, 0x00));
    serializer.insert<uint8_t>(0xDC, 8); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x00));
    serializer.insert<uint8_t>(0x0D, 8); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x0D, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_16bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    size_t nbBitsToSerialize = 28;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint16_t>(0x0003,  4); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00));
    serializer.insert<uint16_t>(0x0001,  2); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00));
    serializer.insert<uint16_t>(0x007F,  8); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xC0, 0x00));
    serializer.insert<uint16_t>(0x3703, 14); ASSERT_THAT(buffer, ElementsAre(0x00, 0x00, 0x00, 0x00, 0x03, 0x5F, 0xF7, 0x03));
}

TEST(BitsInsertion, Unsigned_32bits_with_offset_empty_buffer)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    size_t nbBitsToSerialize = 54;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint32_t>(0x00000003,  4); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x00000001,  2); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x0000007F,  8); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x00003703, 14); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0xDC, 0x0C, 0x00, 0x00, 0x00));
    serializer.insert<uint32_t>(0x017FDC0D, 26); ASSERT_THAT(buffer, ElementsAre(0x00, 0x0D, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_8bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    size_t nbBitsToSerialize = 30;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint8_t>(0x03, 4); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0XFF));
    serializer.insert<uint8_t>(0x01, 2); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0xFF, 0xFF, 0XFF));
    serializer.insert<uint8_t>(0x7F, 8); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xFF, 0XFF));
    serializer.insert<uint8_t>(0xDC, 8); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0XFF));
    serializer.insert<uint8_t>(0x0D, 8); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xCD, 0x7F, 0xDC, 0x0D));
}

TEST(BitsInsertion, Unsigned_16bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    size_t nbBitsToSerialize = 28;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint16_t>(0x0003,  4); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x0001,  2); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x007F,  8); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xFF, 0xFF));
    serializer.insert<uint16_t>(0x3703, 14); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x5F, 0xF7, 0x03));
}

TEST(BitsInsertion, Unsigned_32bits_with_offset_no_empty_buffer)
{
    uint8_t buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    size_t nbBitsToSerialize = 54;
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8, (BUFFER_SIZE * 8) - nbBitsToSerialize);

    serializer.insert<uint32_t>(0x00000003,  4); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x00000001,  2); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x0000007F,  8); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x00003703, 14); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xDC, 0x0F, 0xFF, 0xFF, 0xFF));
    serializer.insert<uint32_t>(0x017FDC0D, 26); ASSERT_THAT(buffer, ElementsAre(0xFF, 0xCD, 0x7F, 0xDC, 0x0D, 0x7F, 0xDC, 0x0D));
}
