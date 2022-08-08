`bits` - _Manipulate low-level bits from high-level abstractions_
=================================
<!-- BEGIN BADGES -->
[![Github License](https://img.shields.io/github/license/jaydee-io/bits)](https://github.com/jaydee-io/bits/blob/main/LICENSE)
[![GitHub Release (latest by date)](https://img.shields.io/github/v/release/jaydee-io/bits)](https://github.com/jaydee-io/bits/releases/latest)
[![Codecov Code coverage](https://img.shields.io/codecov/c/gh/jaydee-io/bits?logo=codecov)](https://codecov.io/gh/jaydee-io/bits)

[![GitHub Workflow Status (branch)](https://img.shields.io/github/workflow/status/jaydee-io/bits/Build%20on%20Ubuntu?label=Ubuntu%20build&logo=ubuntu)](https://github.com/jaydee-io/bits/actions/workflows/build-ubuntu.yml?query=branch%3Amaster)
<!-- END   BADGES -->

## What is `bits`?
`bits` is a C++ 20 library designed to ease low-level bits manipulation from expressive high-level abstractions.

In details, `bits` allows :
- inserting or extracting arbitrary size bits fields into/from raw `std::byte` buffer ([detail](#bits-insertion--extraction))
- arbitrary size bits fields streaming to/from raw `std::byte` buffer (like `std:ostream` and `std::istream`) ([detail](#bits-streaming))
- handling flag bits (set or not set bits) that can be combined within a bits field ([detail](#flags))
- strongly types enumerations with associated helpers ([detail](#enumeration))

:heavy_exclamation_mark:**DISCLAIMER** : This a still a beta project for now and the API is subject to change until the `V1.0` release.:heavy_exclamation_mark:

See all the details of the latest version from [CHANGELOG](CHANGELOG.md).

## TODO
There's a lot in progress :-)

Have a look at the [TODO](TODO.md) list.

---

## Bits insertion / extraction
`bits` provides several `insert()` functions overload to easily insert bits into raw buffer (anything convertible to `std::span<std::byte>`). `bits` also provides several `extract()` functions overload to easily extract bits from raw buffer.

In particular, `insert()` and `extract()` overloads are provided for ranges :
- bounded C style arrays `T[N]`
- C++ arrays `std::array<T, N>`
- C++ spans `std::span<T, N>`
- C++ vector `std::vector<T>`
- ...

In fact, all types satisfying the `std::ranges::output_range` (for extraction) or `std::ranges::input_range` (for insertion) concepts.
Notice that for range type insertion / extraction:
* The number of bits for each range's element could be specified (default to `sizeof(T) * 8`)
* The bits range `[high, low]` should cover the _Number of elements x Number of bits per element_

Be aware that bits range parameters order is `high` first then `low`. The behaviour is undefined if order of parameters `high` and `low` is inverted.

```c++
#include <bits/bits_insertion.h>

template<typename T>
constexpr void insert(const std::span<std::byte> buffer, T val, size_t high, size_t low);

template<std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last, size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */);
template<std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */);



#include <bits/bits_extraction.h>

template<typename T>
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low);

template<typename T>
constexpr void extract(const std::span<const std::byte> buffer, T & val, size_t high, size_t low);

template<output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last, size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */);
template<output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r, size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */);
```

If `high` and `low` (and `nbBitsByElement` for ranges) are known at compile time, they could be specified as template parameters.

```c++
#include <bits/bits_insertion.h>

template<typename T, size_t high, size_t low>
constexpr void insert(T val, const std::span<std::byte> buffer);

template<size_t high, size_t low, typename T>
constexpr void insert(const std::span<std::byte> buffer, T val);

template<size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */, std::input_iterator I, std::sentinel_for<I> S>
constexpr void insert(const std::span<std::byte> buffer, I first, S last);
template<size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */, std::ranges::input_range R>
constexpr void insert(const std::span<std::byte> buffer, R && r);



#include <bits/bits_extraction.h>

template<size_t high, size_t low, typename T>
constexpr T extract(const std::span<const std::byte> buffer);

template<size_t high, size_t low, typename T>
constexpr void extract(const std::span<const std::byte> buffer, T & val);

template<size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */, detail::output_iterator O, std::sentinel_for<O> S>
constexpr void extract(const std::span<const std::byte> buffer, O first, S last);
template<size_t high, size_t low, size_t nbBitsByElement = /* default to size of element */, detail::output_range R>
constexpr void extract(const std::span<const std::byte> buffer, R && r);
```

View some usage examples :
- [Hardware register access](doc/Example_Insertion_Extraction.md#example-hardware-register-access)

## Bits streaming
`bits` offers handy bits streaming classes : `BitsSerializer` to chains bits insertions and `BitsDeserializer` to chains bits extractions.

`insert()` and `extract()` methods could be used to respectively insert and extract bits.

These classes also provides methods :
- `nbBitsStreamed()` to get the number of bits currently streamed (excluding initial offset)
- `skip(size_t nbBits)` to skip a specified number of bits
- `reset()` to reset the stream at the begining. _This reset the stream position to initial offset (`0` if not specified)._

```c++
#include <bits/BitsSerializer.h>

class BitsSerializer
{
public:
    inline BitsSerializer(const std::span<std::byte> buffer, size_t initialOffsetBits = 0);

    template<typename T>    inline BitsSerializer & insert(T val, size_t nbBits = sizeof(T) * CHAR_BIT);
    template<input_range R> inline BitsSerializer & insert(R && r, size_t nbBits = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

    inline size_t nbBitsStreamed(void);

    inline BitsSerializer & skip(size_t nbBits);
    inline BitsSerializer & reset(void);
};
```

```c++
#include <bits/BitsDeserializer.h>

class BitsDeserializer
{
public:
    inline BitsDeserializer(const std::span<const std::byte> buffer, size_t initialOffsetBits = 0);

    template<typename T>     inline T extract(size_t nbBits = sizeof(T) * CHAR_BIT);
    template<typename T>     inline BitsDeserializer & extract(T & val, size_t nbBits = sizeof(T) * CHAR_BIT);
    template<output_range R> inline BitsDeserializer & extract(R && r, size_t nbBits = sizeof(std::ranges::range_value_t<R>) * CHAR_BIT);

    inline size_t nbBitsStreamed(void);

    inline BitsSerializer & skip(size_t nbBits);
    inline BitsSerializer & reset(void);

};
```
In addition to members functions, `bits` provides _streaming operators_ free standing functions `operator <<` and `operator >>`. Free standing functions are also provided to manipulate stream state : number of bits to insert/extract/skip and stream reseting.

```c++
template<typename T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val);
template<input_range R>
inline BitsSerializer & operator <<(BitsSerializer & bs, R && r);
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip);

template<typename T>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T && val);
template<output_range R>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, R && r);
inline BitsDeserializer & operator >>(BitsDeserializer & bs, const detail::BitsStreamManipulation manip);

inline detail::BitsStreamManipulation nbits(size_t nbBits);
inline detail::BitsStreamManipulation skip(size_t nbBits);
inline detail::BitsStreamManipulation reset(void);
```

View some usage examples :
- [Message (de)serialization](doc/Example_Streaming.md#example-message-de-serialization)
- [TCP/IP Packet deserialization](doc/Example_Streaming.md#example-tcp-ip-packet-deserialization)

## Flags
The `Flags` wrapper type helps handling flags, that is a set of bits that could bet set/unsed and tested using a convenient name from a strongly typed enum.
As a wrapper over a strongly typed enumeration, `Flags` provides all relationnal, logical, bitwise and assignment operators as well as casting to `bool` and underlying strongly typed enumeration.
It also provides _expressive_ member functions :
- `set()`, `unset()` and `toggle()` to write the specified flag bit
- `isSet()` and `isNotSet()` to read the specified flag bit

```c++
template<typename EnumType>
class Flags
{
public:
    // Constructors
    constexpr Flags(void) noexcept = default;
    constexpr Flags(EnumType val) noexcept;
    constexpr Flags(const Flags & val) noexcept = default;
    constexpr explicit Flags(MaskType val) noexcept;

    // Relationnal operators
    constexpr auto operator <=>(const Flags & rhs) const noexcept;

    // Logical operator
    constexpr bool operator !(void) const noexcept;

    // Bitwise operators
    constexpr Flags operator &(const Flags & rhs) const noexcept;
    constexpr Flags operator |(const Flags & rhs) const noexcept;
    constexpr Flags operator ^(const Flags & rhs) const noexcept;
    constexpr Flags operator ~(void)              const noexcept;

    // Assignment operators
    constexpr Flags & operator  =(const Flags & rhs) noexcept;
    constexpr Flags & operator &=(const Flags & rhs) noexcept;
    constexpr Flags & operator |=(const Flags & rhs) noexcept;
    constexpr Flags & operator ^=(const Flags & rhs) noexcept;

    // Cast operators
    explicit constexpr operator bool    (void) const noexcept;
    explicit constexpr operator MaskType(void) const noexcept;

    // Basic flag setting and checking
    constexpr void set     (const Flags & val) noexcept;
    constexpr void unset   (const Flags & val) noexcept;
    constexpr void toggle  (const Flags & val) noexcept;
    constexpr bool isSet   (const Flags & val) const noexcept;
    constexpr bool isNotSet(const Flags & val) const noexcept;
};
```

Additionnaly, `bits` provides some helper macros to easily declare a flag and the corresponding strongly type enumeration. This declares :
- a strongly typed enum type, named _`name`_, defined with all the pairs of _name_ / _bit position_ specified. The enumerated value is equal to `1 << bit_position`
- a flag wrapper class alias named `Flags`_`name`_
- a free standing function `bits::to_string()` to convert flags value to string (example: "`{ VAL_1 | BIT_4 }`" or "`{}`")
- all helpers defined for `bits` strong enumeration (See [Enumeration](#enumeration))


```c++
#include <bits/Flags.h>

#define BITS_DECLARE_FLAGS                          (name, ...)                     [...]
#define BITS_DECLARE_FLAGS_WITH_TYPE                (name, rawType, ...)            [...]
#define BITS_DECLARE_FLAGS_WITH_NAMESPACE           (nameSpace, name, ...)          [...]
#define BITS_DECLARE_FLAGS_WITH_TYPE_AND_NAMESPACE  (nameSpace, name, rawType, ...) [...]

#define BITS_DECLARE_FLAGS[...]
(
    [NameSpace]   // declare enum and flags types into namespace
    Name,         // declare an enum <Name> and a flags type named Flags<Name>
    [rawType],    // optional underlying enum type
    VAL_1, 2      // declare an enum value named 'VAL_1' with third bit set : 0b000100
    BIT_4, 4      // declare an enum value named 'BIT_4' with fifth bit set : 0b010000
    ...
)

inline std::string bits::to_string(nameSpace::FlagsName value);
```

View some usage examples :
- [TCP/IP Packet deserialization](doc/Example_Streaming.md#example-tcp-ip-packet-deserialization)

## Enumeration
`bits` provides some helper macros to easily declare a strongly type enumeration. This declares :
- a strongly typed enum type, named _`name`_, defined with all the pairs of _name_ / _value_ specified.
- a free standing function `bits::to_string()` to convert strongly typed enumeration value to string_view (example: "`VAL_1`" or "`<invalid>`")
- a free standing template function specialization `bits::size<>()` to get the number of enumerations
- a free standing template function specialization `bits::names<>()` to get all enumeration's names
- a free standing template function specialization `bits::values<>()` to get all enumeration's values

```c++
#include <bits/Enum.h>

#define BITS_DECLARE_ENUM                          (name, ...)                     [...]
#define BITS_DECLARE_ENUM_WITH_TYPE                (name, rawType, ...)            [...]
#define BITS_DECLARE_ENUM_WITH_NAMESPACE           (nameSpace, name, ...)          [...]
#define BITS_DECLARE_ENUM_WITH_TYPE_AND_NAMESPACE  (nameSpace, name, rawType, ...) [...]

#define BITS_DECLARE_ENUM[...]
(
    Name,            // declare an enum <Name>
    [Type],          // optional underlying enum type
    VAL_1, 0x32      // declare an enum value named 'VAL_1' with value '0x32'
    VAL_4, (4 * 256) // declare an enum value named 'VAL_4' with value 1024 (4 * 256)
    ...
)

inline constexpr std::string_view bits::to_string(Enum value);
template<> inline constexpr size_t bits::size<Enum>(void);
template<> inline constexpr std::array<std::string_view, bits::size<Enum>()> bits::names<Enum>(void);
template<> inline constexpr std::array<Enum, bits::size<Enum>()> bits::values<Enum>(void);
```

View some usage examples :
- [TCP/IP Packet deserialization](doc/Example_Streaming.md#example-tcp-ip-packet-deserialization)
