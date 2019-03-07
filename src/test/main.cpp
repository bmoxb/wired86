#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include "primitives.hpp"
#include "convert.hpp"
#include "emu/memory.hpp"

TEST_CASE("Tests conversions.", "[conversions]") {
    using namespace convert;

    SECTION("Test the fetching of high low bytes of 16-bit values.") {
        REQUIRE(getHighByte(0) == 0);
        REQUIRE(getHighByte(0xAB) == 0);
        REQUIRE(getHighByte(0xABCD) == 0xAB);

        REQUIRE(getLowByte(0) == 0);
        REQUIRE(getLowByte(0xAB) == 0xAB);
        REQUIRE(getLowByte(0xABCD) == 0xCD);
    }
}

TEST_CASE("Test emulator memory.", "[emu][memory]") {
    using Address = u32;
    using Value = u16;
    using Memory = emu::Memory<Value, Address>;

    Memory memory(0xF);

    SECTION("Ensure accurate checking of whether an address is out of bounds.") {
        REQUIRE(memory.withinBounds(0));
        REQUIRE(memory.withinBounds(memory.size - 1));
        REQUIRE_FALSE(memory.withinBounds(memory.size));
    }

    SECTION("Test read/writing of memory.") {
        const Value value = 123;

        for(Address addr = 0; addr < memory.size; addr++) {
            memory.write(addr, value);
            REQUIRE(memory.read(addr) == value);
        }

        REQUIRE_THROWS_AS(memory.write(memory.size, value), Memory::OutOfBounds);
        REQUIRE_THROWS_AS(memory.read(memory.size), Memory::OutOfBounds);
    }

    SECTION("Test reading/writing of multiple values from/to memory.") {
        std::vector<Value> values = { 12, 34, 56 };

        Address addr = memory.size - values.size() - 1;
        memory.write(addr, values);
        REQUIRE(memory.read(addr, values.size()) == values);

        REQUIRE_THROWS_AS(memory.write(memory.size - 2, values), Memory::OutOfBounds);
        REQUIRE_THROWS_AS(memory.read(memory.size - 3, 5), Memory::OutOfBounds);
    }

    SECTION("Test filling of all memory.") {
        const Value value = 456;

        memory.fill(value);

        for(Address addr = 0; addr < memory.size; addr++) {
            REQUIRE(memory.read(addr) == value);
        }
    }
}