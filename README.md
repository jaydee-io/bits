## What is `bits`?
`bits` is a C++ 17 library designed to ease bit manipulations from raw buffer. It allow easy inserting and extracting bit fields into/from raw `uint8_t` buffer.

See all the details of the latest version from [CHANGELOG](https://github.com/jaydee-io/bits/blob/master/CHANGELOG.md).

## Example #1 : _Hardware register access_
`bits` could be very usefull to manipulate low-level hardware bits, extract meaningfull information and raise up abstraction level.

As an example, the following code show how to configure and retrieve values from a [BME680 Environmental Sensor](https://www.bosch-sensortec.com/bst/products/all_products/bme680) from Bosch Sensortec ([Datasheet available here](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001.pdf)).

#### Reading registers
Below is an extract of the datasheet showing part of the Memory Map (page 28).

![BME680 Memory Map](https://github.com/jaydee-io/bits/raw/master/doc/BST-BME680-DS001-28-1.jpg)

To demonstrate bits extraction, we will focus on `gas_r` registers, particulary the `gas_r` and `gas_valid_r` fields, accessible from I2C address `0x2A` to `0x2B`. The datasheet defines this fields as follow (pages 35 and 37).

![gas_r registers definition 1](https://github.com/jaydee-io/bits/raw/master/doc/BST-BME680-DS001-35.jpg)
![gas_r registers definition 2](https://github.com/jaydee-io/bits/raw/master/doc/BST-BME680-DS001-37.jpg)

The following code shows how to extract this fields using `bits`.

```c++
std::array<uint8_t, 2> buffer;

// ... Read 2 bytes from I2C bus, starting at address 0x2A, into the buffer ...

enum class GasValid
{
    INVALID = 0,
    VALID = 1,
};

auto gas_resistance_data  = bits::extract<uint16_t,  9,  0>(buffer.data());
auto gas_is_valid_as_enum = bits::extract<GasValid, 10, 10>(buffer.data());
auto gas_is_valid_as_bool = bits::extract<bool,     10, 10>(buffer.data());

if(gas_is_valid_as_bool)
    std::cout << "Gas value = " << gas_resistance_data << std::endl;
else
    std::cout << "Gas value NOT valid" << std::endl;

if(gas_is_valid_as_enum == GasValid::VALID)
    std::cout << "Gas value = " << gas_resistance_data << std::endl;
else
    std::cout << "Gas value NOT valid" << std::endl;
```

From this code, we could notice 2 things:
* The `gas_valid` field can be extracted either directly as a `boolean` or as a strong type like the `GasValidType` enumeration class.
* The `gas_resistance` field span accross two registers. Notice the bit numbers starting from 0 (first byte) to 9 (second byte). But this is completly transparent to `bits`. Fields could span on any number of bytes.

#### Writing registers
Below is another extract of the datasheet showing antoher part of the Memory Map (page 28).

![BME680 Memory Map](https://github.com/jaydee-io/bits/raw/master/doc/BST-BME680-DS001-28-2.jpg)

For bits insertion, we will focus on `ctrl_meas` register, particulary the `osrs_t` field, accessible from I2C address `0x74`. The datasheet defines this field as follow (pages 30).

![gas_r registers definition 1](https://github.com/jaydee-io/bits/raw/master/doc/BST-BME680-DS001-30.jpg)

The following code show how to insert this field.

```c++
std::array<uint8_t, 1> buffer = { 0x00 };

// ... Read 1 byte from I2C bus, at address 0x74, into the buffer ...

// ... Update the temperature oversampling setting ...
enum class Oversampling
{
    NO_OVERSAMPLING = 0,
    OVERSAMPLING_X1 = 1,
    OVERSAMPLING_X2 = 2,
    OVERSAMPLING_X4 = 3,
    OVERSAMPLING_X8 = 4,
    OVERSAMPLING_X16 = 5,
};

bits::insert<Oversampling, 7, 5>(Oversampling::OVERSAMPLING_X4, buffer.data());

// ... Write back the byte on I2C bus, at address 0x74 ...
```
Obviously, the register must be read before updating the field. This is due to the fact that we don't want to modify the two other fields `osrs_p` and `mode` in the same register.

## TODO
There's a lot in progress :-)
Have a look at the [TODO](https://github.com/jaydee-io/bits/blob/master/TODO.md) list.
