## TODO List
- Merge `BitsSerializer` and `BitsDeserializer` to `BitsStream` ?
- Add a `bits::pos(high, low)` to support `operator <<` and  `operator >>`
- Add `operator <<` to `BitsSerializer`
- Add `operator >>` to `BitsDeserializer`
- Add tests of `assert()` calls
- Add tests of exception throw
- Add tests for signed types
- Replace `uint8_t` by `std::byte`