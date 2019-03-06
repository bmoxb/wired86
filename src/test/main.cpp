#include <vector>

#include "testing.hpp"

#include "primitives.hpp"
#include "logging.hpp"
#include "conversion.hpp"
#include "emu/memory.hpp"

void testMemory() {
    using Address = u32;
    using Value = u16;
    using Memory = emu::Memory<Value, Address>;

    Memory memory(0xAA);

    UnitTest<bool, Address, Value>("Test reading/writing of memory.",
        [&](auto addr, auto value) {
            memory.write(addr, value);
            return memory.read(addr) == value;
        }
    ).test(true, 0, 12).test(true, 34, 56).testForException<Memory::OutOfBounds>(0xAA, 10);

    UnitTest<bool, Address, std::vector<Value>>("Test read/writing of multiple values from/to memory.",
        [&](auto addr, auto values) {
            memory.write(addr, values);
            return memory.read(addr, values.size()) == values;
        }
    ).testNoArgsDisplay(true, 10, { 12, 34, 56 }).testForException<Memory::OutOfBounds>(0xAA - 1, { 12, 34, 56 });

    UnitTest<bool, Address>("Test within bounds checks for memory.",
        [&](auto addr) {
            return memory.withinBounds(addr);
        }
    ).test(true, 0xAA - 1).test(false, 0xAA);
}

void testConversions() {
    UnitTest<bool, u16, u8>("Test conversion::getHighByte function.",
        [&](auto full, auto high) {
            return conversion::getHighByte(full) == high;
        }
    ).test(true, 0, 0).test(true, 0xBB, 0).test(true, 0xAABB, 0xAA);

    UnitTest<bool, u16, u8>("Test conversion::getLowByte function.",
        [&](auto full, auto low) {
            return conversion::getLowByte(full) == low;
        }
    ).test(true, 0, 0).test(true, 0xBB, 0xBB).test(true, 0xAABB, 0xBB);
}

int main(int argc, char* argv[]) {
    try {
        testMemory();
        testConversions();
    }
    catch(UnitTestFailed&) {
        logging::error("Unit tests failed to complete successfully!");
        return -1;
    }

    logging::success("All unit tests passed!");
    return 0;
}