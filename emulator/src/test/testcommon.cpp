#include "catch.hpp"
#include "convert.hpp"

TEST_CASE("Tests conversions.", "[conversions]") {
    using namespace convert;

    SECTION("Test the fetching of high and low bytes from 16-bit values.") {
        REQUIRE(getMostSigByte(0) == 0);
        REQUIRE(getMostSigByte(0xAB) == 0);
        REQUIRE(getMostSigByte(0xABCD) == 0xAB);

        REQUIRE(getLeastSigByte(0) == 0);
        REQUIRE(getLeastSigByte(0xAB) == 0xAB);
        REQUIRE(getLeastSigByte(0xABCD) == 0xCD);
    }

    SECTION("Test the creation of 16-bit values from a high/low 8-bit byte.") {
        REQUIRE(createWordFromBytes(0, 0) == 0);
        REQUIRE(createWordFromBytes(0xAB, 0) == 0xAB);
        REQUIRE(createWordFromBytes(0, 0xCD) == 0xCD00);
        REQUIRE(createWordFromBytes(0xAB, 0xCD) == 0xCDAB);
    }

    SECTION("Test function templates for fetching specific bits of numerical values.") {
        REQUIRE(getBitFrom<u8>(0b10110, 4));
        REQUIRE_FALSE(getBitFrom<u8>(0b101, 1));

        REQUIRE(getBitsFrom<u16>(0b101010100, 2, 7) == 0b1010101);
        REQUIRE(getBitsFrom<u8>(0b10111000, 3, 3) == 0b111);
    }

    SECTION("Test conversion from a numerical value to a hexadecimal string representation.") {
        REQUIRE(toHexString<u8>(0xFF) == "FF");
        REQUIRE(toHexString<u16>(0x55A, "", "h") == "55Ah");
    }

    SECTION("Test conversion from a numerical value to a binary string representation.") {
        REQUIRE(toBinaryString<8, u8>(0b10101010) == "10101010");
        REQUIRE(toBinaryString<4, u16>(0xFF, "", "b") == "1111b");
    }

    SECTION("Test the extension of vectors.") {
        std::vector<u16> vec = { 0xA, 0xB },
                         extendBy = { 0xC, 0xD, 0xE },
                         expected = { 0xA, 0xB, 0xC, 0xD, 0xE };
        
        extendVector<u16>(vec, extendBy);
        REQUIRE(vec == expected);
    }

    SECTION("Test conversion from vector to string.") {
        std::vector<u16> values = { 0xAA, 0xBB, 0xCC };
        std::function<std::string(u16)> convert = [](auto value) { return toHexString(value); };
        
        REQUIRE(vectorToString(values, convert, ", ") == "AA, BB, CC");
    }

    SECTION("Test conversion from hexadecimal string representation to numeric type.") {
        REQUIRE(fromHexString<unsigned int>("0xFF") == 0xFF);
        REQUIRE(fromHexString<unsigned int>("    ab0  ignored") == 0xAB0);
        REQUIRE(fromHexString<unsigned long long>("\n\tabcdef123") == 0xABCDEF123);
    }

    SECTION("Test conversion from decimal string representation to a numeric type.") {
        REQUIRE(fromBinaryString<unsigned int>("1010") == 0b1010);
        REQUIRE(fromBinaryString<unsigned char>("   1111  abc") == 0b1111);
    }
}