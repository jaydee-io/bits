////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>

#include <bits/BitsDeserializer.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

//-----------------------------------------------------------------------------
//- Serializer / Deserializer common tests
//-----------------------------------------------------------------------------
TEST(BitsDeserializer, BitsSkipped)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsDeserializer, ChainedSkip)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer stream(buffer);

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