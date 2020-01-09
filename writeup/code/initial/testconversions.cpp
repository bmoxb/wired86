TEST_CASE("Tests conversions.", "[conversions]") {
    using namespace conversion;

    SECTION("Test the fetching of high and low bytes from 16-bit values.") {
        REQUIRE(getHighByte(0) == 0);
        REQUIRE(getHighByte(0xAB) == 0);
        REQUIRE(getHighByte(0xABCD) == 0xAB);

        REQUIRE(getLowByte(0) == 0);
        REQUIRE(getLowByte(0xAB) == 0xAB);
        REQUIRE(getLowByte(0xABCD) == 0xCD);
    }
}
