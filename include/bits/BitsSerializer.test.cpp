////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>
#include <span>
#include <vector>
#include <list>
#include <cstddef>

#include <bits/BitsSerializer.h>

using ::testing::ElementsAreArray;

template<typename T = std::byte, typename... Ts>
constexpr std::array<T, sizeof...(Ts)> make_array(Ts && ... args) noexcept
{
    return { T(std::forward<Ts>(args))... };
}

const size_t BUFFER_SIZE = 8;

//-----------------------------------------------------------------------------
//- Serializer / Deserializer common tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, BitsSkipped)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);

    stream.reset();
    ASSERT_EQ(stream.nbBitsStreamed(), 0);
}

TEST(BitsSerializer, ChainedSkip)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream
        .skip(4)
        .skip(2)
        .skip(8)
        .skip(8)
        .skip(8);

    ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

TEST(BitsSerializer, OutOfRange)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_THROW(stream.skip(128), std::out_of_range);
}

//-----------------------------------------------------------------------------
//- BitsSerializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, NbBitsSerialized)
{
    auto buffer = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
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
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert(uint8_t { 0x03 }, 4)
        .insert(uint8_t { 0x01 }, 2)
        .insert(uint8_t { 0x7F }, 8)
        .insert(uint8_t { 0xDC }, 8)
        .insert(uint8_t { 0x0D }, 8)
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsSerializer, ChainedInsert_AutoSize)
{
    std::array<std::byte, BUFFER_SIZE * 2> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert<uint8_t> (0xD8)
        .insert<uint8_t> (0xE9)
        .insert<uint16_t>(0xA5A5)
        .insert<uint32_t>(0xB6B6'B6B6)
        .insert<uint64_t>(0xC7C7'C7C7'C7C7'C7C7)
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7)));
}

TEST(BitsSerializer, ChainedInsert_Operator)
{
    std::array<std::byte, BUFFER_SIZE * 2> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        << uint8_t  { 0xD8 }
        << uint8_t  { 0xE9 }
        << uint16_t { 0xA5A5 }
        << uint32_t { 0xB6B6'B6B6 }
        << uint64_t { 0xC7C7'C7C7'C7C7'C7C7 }
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7)));
}

TEST(BitsSerializer, BitsManipulation_nbits)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        << bits::nbits(4) << uint8_t { 0x03 }     // 4 bits serialized
        << bits::nbits(2) << uint8_t { 0x01 }     // 2 bits serialized
        << bits::nbits(4) << uint16_t { 0x07 }    // 4 bits serialized
        << bits::nbits(2) << uint32_t { 0x03 }    // 2 bits serialized
        << bits::nbits(4) << uint64_t { 0x0F }    // 4 bits serialized
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsSerializer, BitsManipulation_SkipAndReset)
{
    auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsSerializer serializer(buffer, 8);

    serializer << bits::skip(8);
    ASSERT_EQ(serializer.nbBitsStreamed(), 8);

    serializer << bits::reset();
    ASSERT_EQ(serializer.nbBitsStreamed(), 0);

    serializer << uint8_t(0xAA);
    ASSERT_EQ(buffer[1], std::byte(0xAA));
}

TEST(BitsSerializer, Ranges)
{
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        uint8_t c_array[3] = { 0x5F, 0xF7, 0x03 };
        serializer << c_array;
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::array<uint8_t, 3> std_array = { 0x5F, 0xF7, 0x03 };
        serializer << std_array;
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::array<uint8_t, 3> std_array_for_span = { 0x5F, 0xF7, 0x03 };
        std::span<uint8_t, 3> std_span(std_array_for_span);
        serializer << std_span;
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::vector<uint8_t> std_vector = { 0x5F, 0xF7, 0x03 };
        serializer << std_vector;
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::list<uint8_t> std_list = { 0x5F, 0xF7, 0x03 };
        serializer << std_list;
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
}

TEST(BitsSerializer, Ranges_Subranges)
{
    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        uint8_t c_array[6] = { 0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00 };
        serializer << std::ranges::subrange(std::ranges::next(std::ranges::begin(c_array)), std::ranges::next(std::ranges::begin(c_array), 4));
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }

    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::array<uint8_t, 6> std_array = { 0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00 };
        serializer << std::ranges::subrange(std::ranges::next(std::ranges::begin(std_array)), std::ranges::next(std::ranges::begin(std_array), 4));
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }

    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::array<uint8_t, 6> std_array = { 0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00 };
        std::span<uint8_t, 6> std_span(std_array);
        serializer << std::ranges::subrange(std::ranges::next(std::ranges::begin(std_span)), std::ranges::next(std::ranges::begin(std_span), 4));
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }

    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::vector<uint8_t> std_vector = { 0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00 };
        serializer << std::ranges::subrange(std::ranges::next(std::ranges::begin(std_vector)), std::ranges::next(std::ranges::begin(std_vector), 4));
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }

    {
        auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        bits::BitsSerializer serializer(buffer, 4);
        std::list<uint8_t> std_list = { 0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00 };
        serializer << std::ranges::subrange(std::ranges::next(std::ranges::begin(std_list)), std::ranges::next(std::ranges::begin(std_list), 4));
        ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0xFF, 0x70, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF)));
    }
}

TEST(BitsSerializer, Ranges_nbits)
{
    auto buffer = make_array(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    bits::BitsSerializer serializer(buffer, 4);
    uint8_t c_array[3] = { 0x05, 0x07, 0x03 };
    serializer << bits::nbits(4) << c_array;
    ASSERT_THAT(buffer, ElementsAreArray(make_array(0xF5, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)));
}
