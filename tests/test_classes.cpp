#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/BitsSerializer.h>
#include <bits/BitsDeserializer.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

TEST(BitsSerializer, NbBitsSerialized)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(serializer.nbBitsStreamed(), 0);
    serializer.insert<uint8_t>(0x0, 4); ASSERT_EQ(serializer.nbBitsStreamed(), 4);
    serializer.insert<uint8_t>(0x0, 2); ASSERT_EQ(serializer.nbBitsStreamed(), 6);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 14);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 22);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 30);
}

TEST(BitsDeserializer, NbBitsDeserialized)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);

    ASSERT_EQ(deserializer.nbBitsStreamed(), 0);
    deserializer.extract<uint8_t>(4); ASSERT_EQ(deserializer.nbBitsStreamed(), 4);
    deserializer.extract<uint8_t>(2); ASSERT_EQ(deserializer.nbBitsStreamed(), 6);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 14);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 22);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 30);
}

TEST(BitsStream, BitsSkipped)
{
    bits::detail::BaseBitsStream stream(BUFFER_SIZE * 8, 0);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsSerializer, ChainedInsert)
{
    uint8_t buffer[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer, BUFFER_SIZE * 8);

    serializer
        .insert(0x03, 4)
        .insert(0x01, 2)
        .insert(0x7F, 8)
        .insert(0xDC, 8)
        .insert(0x0D, 8);
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsDeserializer, ChainedExtract)
{
    uint8_t buffer[] = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer, BUFFER_SIZE * 8);
    uint8_t val = 0;

    deserializer
        .extract(val, 4)
        .extract(val, 2)
        .extract(val, 8)
        .extract(val, 8)
        .extract(val, 8);
    
    ASSERT_EQ(val, 0x0D);
}