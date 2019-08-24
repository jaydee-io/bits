#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>

#include <bits/BitsSerializer.h>
#include <bits/BitsDeserializer.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

//-----------------------------------------------------------------------------
//- Serializer / Deserializer common tests
//-----------------------------------------------------------------------------
TEST(BitsStream, BitsSkipped)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer); // Could be BitsDeserializer

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsStream, ChainedSkip)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer stream(buffer); // Could be BitsSerializer

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream
        .skip(4)
        .skip(2)
        .skip(8)
        .skip(8)
        .skip(8);
    
    ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

//-----------------------------------------------------------------------------
//- BitsSerializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, NbBitsSerialized)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    ASSERT_EQ(serializer.nbBitsStreamed(), 0);
    serializer.insert<uint8_t>(0x0, 4); ASSERT_EQ(serializer.nbBitsStreamed(), 4);
    serializer.insert<uint8_t>(0x0, 2); ASSERT_EQ(serializer.nbBitsStreamed(), 6);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 14);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 22);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 30);
}

TEST(BitsSerializer, ChainedInsert)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert(0x03, 4)
        .insert(0x01, 2)
        .insert(0x7F, 8)
        .insert(0xDC, 8)
        .insert(0x0D, 8)
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsSerializer, ChainedInsert_AutoSize)
{
    std::array<uint8_t, BUFFER_SIZE * 2> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert<uint8_t> (0xD8)
        .insert<uint8_t> (0xE9)
        .insert<uint16_t>(0xA5A5)
        .insert<uint32_t>(0xB6B6'B6B6)
        .insert<uint64_t>(0xC7C7'C7C7'C7C7'C7C7)
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7));
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
        << uint8_t { 0x0D }          // 8 bits serialized
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

//-----------------------------------------------------------------------------
//- BitsDeserializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsDeserializer, NbBitsDeserialized)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsDeserializer deserializer(buffer);

    ASSERT_EQ(deserializer.nbBitsStreamed(), 0);
    deserializer.extract<uint8_t>(4); ASSERT_EQ(deserializer.nbBitsStreamed(), 4);
    deserializer.extract<uint8_t>(2); ASSERT_EQ(deserializer.nbBitsStreamed(), 6);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 14);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 22);
    deserializer.extract<uint8_t>(8); ASSERT_EQ(deserializer.nbBitsStreamed(), 30);
}

TEST(BitsDeserializer, ChainedExtract)
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
        .extract(val5, 8)
    ;
    
    ASSERT_EQ(val1, 0x03);
    ASSERT_EQ(val2, 0x01);
    ASSERT_EQ(val3, 0x7F);
    ASSERT_EQ(val4, 0xDC);
    ASSERT_EQ(val5, 0x0D);
}

TEST(BitsDeserializer, ChainedExtract_AutoSize)
{
    std::array<uint8_t, BUFFER_SIZE * 2> buffer = { 0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7 };
    bits::BitsDeserializer deserializer(buffer);
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint16_t val3 = 0;
    uint32_t val4 = 0;
    uint64_t val5 = 0;

    deserializer
        .extract(val1)
        .extract(val2)
        .extract(val3)
        .extract(val4)
        .extract(val5)
    ;
    
    ASSERT_EQ(val1, 0xD8);
    ASSERT_EQ(val2, 0xE9);
    ASSERT_EQ(val3, 0xA5A5);
    ASSERT_EQ(val4, 0xB6B6'B6B6);
    ASSERT_EQ(val5, 0xC7C7'C7C7'C7C7'C7C7);
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
        >> val5                      // 8 bits extracted
    ;
    
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