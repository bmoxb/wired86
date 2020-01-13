SECTION("Test conversion from a numerical value to a hexadecimal string representation.") {
        REQUIRE(toHexString<u8>(0xFF) == "FF");
        REQUIRE(toHexString<u16>(0xABCD, "0x") == "0xABCD");
        REQUIRE(toHexString<u16>(0x55A, "", "h") == "55Ah");
    }

    SECTION("Test conversion from a numerical value to a binary string representation.") {
        REQUIRE(toBinaryString<8, u8>(0b10101010) == "10101010");
        REQUIRE(toBinaryString<6, u8>(0b111111, "0b") == "0b111111");
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
        REQUIRE(fromHexString<u16>("0xFF") == 0xFF);
        REQUIRE(fromHexString<u16>("    ab0  ignored") == 0xAB0);
        REQUIRE(fromHexString<u32>("\n\tabcdef") == 0xABCDEF);
    }

    SECTION("Test conversion from decimal string representation to a numeric type.") {
        REQUIRE(fromBinaryString<u16>("1010") == 0b1010);
        REQUIRE(fromBinaryString<u8>("   1111  abc") == 0b1111);
    }

    SECTION("Test conversion from numerical value to string based on assembly style specified.") {
        assembly::Style s;

        s.numericalRepresentation = assembly::HEX_REPRESENTATION;
        s.numericalStyle = assembly::WITH_PREFIX;
        s.hexPrefix = "0x";

        REQUIRE(numberToAssembly<u16>(0xFF, s) == "0xFF");

        s.numericalRepresentation = assembly::BINARY_REPRESENTATION;
        s.numericalStyle = assembly::WITHOUT_SUFFIX_OR_PREFIX;

        REQUIRE(numberToAssembly<u8, 8>(0b1010, s) == "00001010");

        s.numericalRepresentation = assembly::DENARY_REPRESENTATION;

        REQUIRE(numberToAssembly<u8>(25, s) == "25");
    }
}
