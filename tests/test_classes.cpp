#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/bits.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

TEST(BitsSerializer, NbBitsSerialized)
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

TEST(BitsDeserializer, NbBitsDeserialized)
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