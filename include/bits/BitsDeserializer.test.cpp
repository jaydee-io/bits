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

#include <bits/BitsDeserializer.h>

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
TEST(BitsDeserializer, BitsSkipped)
{
    const std::array<const std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);

    stream.reset();
    ASSERT_EQ(stream.nbBitsStreamed(), 0);
}

TEST(BitsDeserializer, ChainedSkip)
{
    const std::array<const std::byte, BUFFER_SIZE> buffer = {};
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

TEST(BitsDeserializer, OutOfRange)
{
    const std::array<const std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer stream(buffer);

    ASSERT_THROW(stream.skip(128), std::out_of_range);
}

//-----------------------------------------------------------------------------
//- BitsDeserializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsDeserializer, NbBitsDeserialized)
{
    const std::array<const std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsDeserializer deserializer(buffer);

    ASSERT_EQ(deserializer.nbBitsStreamed(), 0);
    deserializer.extract<uint8_t>(4); ASSERT_EQ(deserializer.nbBitsStreamed(), 4);
    deserializer.extract<uint8_t>(2); ASSERT_EQ(deserializer.nbBitsStreamed(), 6);
    deserializer.extract<uint8_t>();  ASSERT_EQ(deserializer.nbBitsStreamed(), 14);
    deserializer.extract<uint8_t>();  ASSERT_EQ(deserializer.nbBitsStreamed(), 22);
    deserializer.extract<uint8_t>();  ASSERT_EQ(deserializer.nbBitsStreamed(), 30);
}

TEST(BitsDeserializer, ChainedExtract)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
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

    ASSERT_EQ(val1, 0x03u);
    ASSERT_EQ(val2, 0x01u);
    ASSERT_EQ(val3, 0x7Fu);
    ASSERT_EQ(val4, 0xDCu);
    ASSERT_EQ(val5, 0x0Du);
}

TEST(BitsDeserializer, ChainedExtract_AutoSize)
{
    const auto buffer = make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7);
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

    ASSERT_EQ(val1, 0xD8u);
    ASSERT_EQ(val2, 0xE9u);
    ASSERT_EQ(val3, 0xA5A5u);
    ASSERT_EQ(val4, 0xB6B6'B6B6u);
    ASSERT_EQ(val5, 0xC7C7'C7C7'C7C7'C7C7u);
}

TEST(BitsDeserializer, ChainedExtract_Operator)
{
    const auto buffer = make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7);
    bits::BitsDeserializer deserializer(buffer);
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint16_t val3 = 0;
    uint32_t val4 = 0;
    uint64_t val5 = 0;

    deserializer
        >> val1
        >> val2
        >> val3
        >> val4
        >> val5
    ;

    ASSERT_EQ(val1, 0xD8u);
    ASSERT_EQ(val2, 0xE9u);
    ASSERT_EQ(val3, 0xA5A5u);
    ASSERT_EQ(val4, 0xB6B6'B6B6u);
    ASSERT_EQ(val5, 0xC7C7'C7C7'C7C7'C7C7u);
}

TEST(BitsDeserializer, BitsManipulation_nbits)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsDeserializer deserializer(buffer);
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint16_t val3 = 0;
    uint32_t val4 = 0;
    uint64_t val5 = 0;

    deserializer
        >> bits::nbits(4) >> val1    // 4 bits extracted
        >> bits::nbits(2) >> val2    // 2 bits extracted
        >> bits::nbits(4) >> val3    // 4 bits extracted
        >> bits::nbits(2) >> val4    // 2 bits extracted
        >> bits::nbits(4) >> val5    // 4 bits extracted
    ;

    ASSERT_EQ(val1, 0x03u);
    ASSERT_EQ(val2, 0x01u);
    ASSERT_EQ(val3, 0x07u);
    ASSERT_EQ(val4, 0x03u);
    ASSERT_EQ(val5, 0x0Fu);
}

TEST(BitsDeserializer, BitsManipulation_SkipAndReset)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsDeserializer deserializer(buffer);

    deserializer >> bits::skip(8);
    ASSERT_EQ(deserializer.nbBitsStreamed(), 8);

    deserializer >> bits::reset();
    ASSERT_EQ(deserializer.nbBitsStreamed(), 0);
}

TEST(BitsDeserializer, Deserialize_size_t)
{
    const auto buffer = make_array(0xCA, 0xFE, 0xBA, 0xBE, 0xA5, 0xB6, 0xC7, 0xD8);
    bits::BitsDeserializer deserializer(buffer);
    size_t val = 0;

    deserializer >> val;
    ASSERT_EQ(val, static_cast<size_t>(0xCAFEBABE'A5B6C7D8)); // On 32bits platform, size_t is 32bits

    deserializer >> bits::reset() >> bits::nbits(8) >> val;
    ASSERT_EQ(val, 0xCA);
}

TEST(BitsDeserializer, Ranges)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsDeserializer deserializer(buffer, 4);
    uint8_t c_array[3] = {};
    std::array<uint8_t, 3> std_array = {};
    std::array<uint8_t, 3> std_array_for_span = {};
    std::span<uint8_t, 3> std_span(std_array_for_span);
    std::vector<uint8_t> std_vector(3, 0);
    std::list<uint8_t> std_list(3, 0);

    deserializer
        >> bits::reset() >> c_array
        >> bits::reset() >> std_array
        >> bits::reset() >> std_span
        >> bits::reset() >> std_vector
        >> bits::reset() >> std_list
    ;

    ASSERT_THAT(c_array, ElementsAreArray(make_array<uint8_t>(0x5F, 0xF7, 0x03)));
    ASSERT_THAT(std_array, ElementsAreArray(make_array<uint8_t>(0x5F, 0xF7, 0x03)));
    ASSERT_THAT(std_span, ElementsAreArray(make_array<uint8_t>(0x5F, 0xF7, 0x03)));
    ASSERT_THAT(std_vector, ElementsAreArray(make_array<uint8_t>(0x5F, 0xF7, 0x03)));
    ASSERT_THAT(std_list, ElementsAreArray(make_array<uint8_t>(0x5F, 0xF7, 0x03)));
}

TEST(BitsDeserializer, Ranges_Subranges)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsDeserializer deserializer(buffer, 4);

    uint8_t c_array[6] = {};
    std::array<uint8_t, 6> std_array = {};
    std::array<uint8_t, 6> std_array_for_span = {};
    std::span<uint8_t, 6> std_span(std_array_for_span);
    std::vector<uint8_t> std_vector(6, 0);
    std::list<uint8_t> std_list(6, 0);

    deserializer
        >> bits::reset() >> std::ranges::subrange(std::next(std::begin(c_array)), std::next(std::begin(c_array), 4))
        >> bits::reset() >> std::ranges::subrange(std::next(std::begin(std_array)), std::next(std::begin(std_array), 4))
        >> bits::reset() >> std::ranges::subrange(std::next(std::begin(std_span)), std::next(std::begin(std_span), 4))
        >> bits::reset() >> std::ranges::subrange(std::next(std::begin(std_vector)), std::next(std::begin(std_vector), 4))
        >> bits::reset() >> std::ranges::subrange(std::next(std::begin(std_list)), std::next(std::begin(std_list), 4))
    ;

    ASSERT_THAT(c_array, ElementsAreArray(make_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
    ASSERT_THAT(std_array, ElementsAreArray(make_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
    ASSERT_THAT(std_span, ElementsAreArray(make_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
    ASSERT_THAT(std_vector, ElementsAreArray(make_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
    ASSERT_THAT(std_list, ElementsAreArray(make_array<uint8_t>(0x00, 0x5F, 0xF7, 0x03, 0x00, 0x00)));
}

TEST(BitsDeserializer, Ranges_nbits)
{
    const auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsDeserializer deserializer(buffer, 4);
    std::array<uint8_t, 6> array = {};

    deserializer >> bits::nbits(4) >> array;
    ASSERT_THAT(array, ElementsAreArray(make_array<uint8_t>(0x05, 0x0F, 0x0F, 0x07, 0x00, 0x03 )));
}