`bits` - _Manipulate low-level bits from high-level abstractions_
=================================
<!-- BEGIN BADGES -->
[![Github License](https://img.shields.io/github/license/jaydee-io/bits)](https://github.com/jaydee-io/bits/blob/main/LICENSE)
[![GitHub Release (latest by date)](https://img.shields.io/github/v/release/jaydee-io/bits)](https://github.com/jaydee-io/bits/releases/latest)
[![Travis Build status](https://img.shields.io/travis/com/jaydee-io/bits)](https://app.travis-ci.com/github/jaydee-io/bits)
[![Codecov Code coverage](https://img.shields.io/codecov/c/github/jaydee-io/bits?token=0aa2561e-906c-4afa-9f12-b667da9435e1)](https://codecov.io/gh/jaydee-io/bits)
<!-- END   BADGES -->

## What is `bits`?
`bits` is a C++ 20 library designed to ease low-level bits manipulation from expressive high-level abstractions.

In details, `bits` allows :
- inserting or extracting arbitrary size bits fields into/from raw `std::byte` buffer ([detail](#bits-insertion-extraction))
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
`bits` provides several `insert()` functions overload to easily insert bits into raw buffer (anything convertible to `std::span<std::byte>`). `bits` also provides several `extract()` functions overload to easily extract bits from raw buffer (anything convertible to `std::span<std::byte>`).

Be aware that bits range parameters order is `high` first then `low`.

The behaviour is undefined if :
- parameters `high` and `low` order is inverted
- `high` or `low` overflow the buffer size

```c++
#include <bits/bits_insertion.h>

template<typename T>
constexpr void insert(T val, const std::span<std::byte> buffer, size_t high, size_t low);



#include <bits/bits_extraction.h>

template<typename T>
constexpr T extract(const std::span<const std::byte> buffer, size_t high, size_t low);
```

If `high` and `low` are known at compile time, they could be specified as template parameters.

```c++
#include <bits/bits_insertion.h>

template<typename T, size_t high, size_t low>
constexpr void insert(T val, const std::span<std::byte> buffer);



#include <bits/bits_extraction.h>

template<typename T, size_t high, size_t low>
constexpr T extract(const std::span<const std::byte> buffer);
```

View some usage examples :
- [Hardware register access](doc/Example_Insertion_Extraction.md#example-hardware-register-access)

## Bits streaming
`bits` offers handy bits streaming classes : `BitsSerializer` to chains bits insertions and `BitsDeserializer` to chains bits extractions.

`insert()` and `extract()` methods could be used to respectively insert and extract bits.

These classes also provides method :
- `nbBitsStreamed()` to get the number of bits currently streamed (excluding initial offset)
- `skip(size_t nbBits)` to skip a specified number of bits
- `reset()` to reset the stream at the begining. _This reset the stream position to initial offset (`0` if not specified)._

```c++
#include <bits/BitsSerializer.h>

class BitsSerializer
{
public:
    inline BitsSerializer(const std::span<std::byte> buffer, size_t lengthBufferBits = N * 8, size_t initialOffsetBits = 0);

    template<typename T>
    inline BitsSerializer & insert(T val, size_t nbBits = sizeof(T) * 8);

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
    inline BitsDeserializer(const std::span<const std::byte> buffer, size_t lengthBufferBits = N * 8, size_t initialOffsetBits = 0);

    template<typename T, std::enable_if_t< ! std::is_same_v<T, BitsDeserializer>, int> = 0>
    inline T extract(size_t nbBits);
    template<typename T>
    inline BitsDeserializer & extract(T & val, size_t nbBits = sizeof(T) * 8);

    inline size_t nbBitsStreamed(void);

    inline BitsSerializer & skip(size_t nbBits);
    inline BitsSerializer & reset(void);

};
```
In addition to members functions, `bits` provides _streaming operators_ free standing functions `operator <<` and `operator >>`. Free standing functions are also provided to manipulate stream state : number of bits to insert/extract/skip and stream reseting.

```c++
template<typename T>
inline BitsSerializer & operator <<(BitsSerializer & bs, T val);
inline BitsSerializer & operator <<(BitsSerializer & bs, const detail::BitsStreamManipulation manip);

template<typename T>
inline BitsDeserializer & operator >>(BitsDeserializer & bs, T & val);
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
- an `EnumTraits` specialization for the strongly typed enumeration, which provides :
  - `bits::EnumTraits<nameSpace::Name>::size()` to get the number of enumerations

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

inline std::string_view bits::to_string(nameSpace::Name value);
inline static constexpr size_t bits::EnumTraits<nameSpace::Name>::size(void);
```

View some usage examples :
- [TCP/IP Packet deserialization](doc/Example_Streaming.md#example-tcp-ip-packet-deserialization)
