TEST_CASE("Test emulator memory.", "[emu][memory]") {
    // Define some type aliases for convenience:
    using Address = u32;
    using Value = u16;
    using Memory = emu::Memory<Value, Address>;

    Memory memory(0xF); // Create instance of memory class which is used by all tests below.

    SECTION("Ensure accurate checking of whether an address is out of bounds.") {
        REQUIRE(memory.withinBounds(0)); // normal
        REQUIRE(memory.withinBounds(memory.size - 1)); // boundary
        REQUIRE_FALSE(memory.withinBounds(memory.size)); // erroneous
    }

    SECTION("Test read/writing of memory.") {
        constexpr Value value = 123;

        // Normal and boundary (as every memory address is written to and read from):
        for(Address addr = 0; addr < memory.size; addr++) {
            memory.write(addr, value);
            REQUIRE(memory.read(addr) == value);
        }

        // Erroneous read/write (exception should be thrown):
        REQUIRE_THROWS_AS(memory.write(memory.size, value), Memory::OutOfBounds);
        REQUIRE_THROWS_AS(memory.read(memory.size), Memory::OutOfBounds);
    }

    SECTION("Test reading/writing of multiple values from/to memory.") {
        std::vector<Value> values = { 12, 34, 56 };

        // Boundary read/write:
        Address addr = memory.size - values.size() - 1;
        memory.write(addr, values);
        REQUIRE(memory.read(addr, values.size()) == values);

        // Erroneous (exception should be thrown):
        REQUIRE_THROWS_AS(memory.write(memory.size - 2, values), Memory::OutOfBounds);
        REQUIRE_THROWS_AS(memory.read(memory.size - 3, 5), Memory::OutOfBounds);
    }

    SECTION("Test filling of all memory.") {
        constexpr Value value = 456;

        memory.fill(value);

        for(Address addr = 0; addr < memory.size; addr++) {
            REQUIRE(memory.read(addr) == value);
        }
    }
}
