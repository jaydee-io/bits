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
    uint8_t buffer[BUFFER_SIZE] = {};
    bits::BitsSerializer stream(buffer, BUFFER_SIZE * 8); // Could be BitsDeserializer

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsStream, ChainedSkip)
{
    uint8_t buffer[BUFFER_SIZE] = {};
    bits::BitsDeserializer stream(buffer, BUFFER_SIZE * 8); // Could be BitsSerializer

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream
        .skip(4)
        .skip(2)
        .skip(8)
        .skip(8)
        .skip(8);
    
    ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsSerializer, CArray_ChainedInsert)
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

TEST(BitsDeserializer, CArray_ChainedExtract)
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

TEST(BitsSerializer, CppArray_ChainedInsert)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert(0x03, 4)
        .insert(0x01, 2)
        .insert(0x7F, 8)
        .insert(0xDC, 8)
        .insert(0x0D, 8);
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsDeserializer, CppArray_ChainedExtract)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer);
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint8_t val3 = 0;
    uint8_t val4 = 0;
    uint8_t val5 = 0;

    deserializer
        .extract(val1, 4)
        .extract(val2, 2)
        .extract(val3, 8)
        .extract(val4, 8)
        .extract(val5, 8);
    
    ASSERT_EQ(val1, 0x03);
    ASSERT_EQ(val2, 0x01);
    ASSERT_EQ(val3, 0x7F);
    ASSERT_EQ(val4, 0xDC);
    ASSERT_EQ(val5, 0x0D);
}

TEST(BitsSerializer, BitsManipulation_ChainedInsert)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        << bits::nbits(4) << 0x03    // 2 bits serialized
        << bits::nbits(2) << 0x01    // 4 bits serialized
        << uint8_t { 0x7F }          // 8 bits serialized
        << uint8_t { 0xDC }          // 8 bits serialized
        << uint8_t { 0x0D };         // 8 bits serialized
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsDeserializer, BitsManipulation_ChainedExtract)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF };
    bits::BitsDeserializer deserializer(buffer);
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint8_t val3 = 0;
    uint8_t val4 = 0;
    uint8_t val5 = 0;

    deserializer
        >> bits::nbits(4) >> val1    // 4 bits extracted
        >> bits::nbits(2) >> val2    // 2 bits extracted
        >> val3                      // 8 bits extracted
        >> val4                      // 8 bits extracted
        >> val5;                     // 8 bits extracted
    
    ASSERT_EQ(val1, 0x03);
    ASSERT_EQ(val2, 0x01);
    ASSERT_EQ(val3, 0x7F);
    ASSERT_EQ(val4, 0xDC);
    ASSERT_EQ(val5, 0x0D);
}

TEST(BitsDeserializer, Deserialize_size_t)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0xCA, 0xFE, 0xBA, 0xBE, 0xA5, 0xB6, 0xC7, 0xD8 };
    bits::BitsDeserializer deserializer(buffer);
    size_t val = 0;

    deserializer >> val;
    
    ASSERT_EQ(val, 0xCAFEBABE'A5B6C7D8);
}