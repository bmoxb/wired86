#include <vector>

#include "testing.hpp"

#include "primitives.hpp"
#include "logging.hpp"
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
    ).test(true, 0, 12).test(true, 34, 56);

    UnitTest<bool, Address, std::vector<Value>>("Test read/writing of multiple values from/to memory.",
        [&](auto addr, auto values) {
            memory.write(addr, values);
            return memory.read(addr, values.size()) == values;
        }
    ).testNoArgsDisplay(true, 10, { 12, 34, 56 });

    UnitTest<bool, Address>("Test within bounds checks for memory.",
        [&](auto addr) {
            return memory.withinBounds(addr);
        }
    ).test(true, 0xAA - 1).test(false, 0xAA);

    UnitTest<Value, Address>("Test throwing of OutOfBounds exception when accessing memory that is out-of-bounds.",
        [&](auto addr) {
            return memory.read(addr);
        }
    ).testForException<Memory::OutOfBounds>(0xAA);
}

int main(int argc, char* argv[]) {
    try {
        testMemory();
    }
    catch(UnitTestFailed&) {
        logging::error("Unit tests failed to complete successfully!");
        return -1;
    }

    logging::success("All unit tests passed!");
    return 0;
}