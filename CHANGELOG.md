## Change log

### Not yet released
- Add range extraction
- Replace `uint8_t[]` and `std::array<uint8_t, ...>` with `std::span<std::byte>`
- Add trait on Enum & Flags : size()
- Add BitsField type
- Add Enum type
- Add Flags type
- Add support for unsigned types
- Fix issue with size_t deserialization
- Add `operator <<` and `operator >>` to bits streams
- Free functions version with `high` and `low` as template parameters
- Free functions for bits insertion / extraction
- Implementation's detail hidden under 'bits::detail' namespace
- Initial version