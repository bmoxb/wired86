#include <vector>

#include "testing.hpp"

#include "primitives.hpp"
#include "logging.hpp"
#include "emu/memory.hpp"

void testMemory() {
    using Address = u32;
    using Value = u16;

    emu::Memory<Value, Address> memory(0xAA);

    UnitTest<Address, Value>("Test reading/writing of memory.",
        [&](auto addr, auto value) {
            memory.write(addr, value);
            return memory.read(addr) == value;
        }
    ).test(0, 12).test(34, 56);

    UnitTest<Address, std::vector<Value>>("Test read/writing of multiple values from/to memory.",
        [&](auto addr, auto values) {
            memory.write(addr, values);
            return memory.read(addr, values.size()) == values;
        }
    ).testNoArgsDisplay(10, { 12, 34, 56 });
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