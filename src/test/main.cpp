#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include "primitives.hpp"
#include "convert.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/registers.hpp"
#include "emu/cpu/instr/opcode.hpp"

TEST_CASE("Tests conversions.", "[conversions]") {
    using namespace convert;

    SECTION("Test the fetching of high and low bytes from 16-bit values.") {
        REQUIRE(getHighByte(0) == 0);
        REQUIRE(getHighByte(0xAB) == 0);
        REQUIRE(getHighByte(0xABCD) == 0xAB);

        REQUIRE(getLowByte(0) == 0);
        REQUIRE(getLowByte(0xAB) == 0xAB);
        REQUIRE(getLowByte(0xABCD) == 0xCD);
    }

    SECTION("Test the creation of 16-bit values from a high/low 8-bit byte.") {
        REQUIRE(createWordFromBytes(0, 0) == 0);
        REQUIRE(createWordFromBytes(0xAB, 0) == 0xAB);
        REQUIRE(createWordFromBytes(0, 0xCD) == 0xCD00);
        REQUIRE(createWordFromBytes(0xAB, 0xCD) == 0xCDAB);
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

TEST_CASE("Test CPU registers.", "[emu][cpu][registers]") {
    enum Index { REG };
    emu::cpu::Registers<Index, u32> regs;

    SECTION("Ensure registers are initialised to 0.") {
        REQUIRE(regs.get(REG) == 0);
    }

    SECTION("Test setting/getting register values.") {
        regs.set(REG, 0xBED);
        REQUIRE(regs.get(REG) == 0xBED);
    }

    emu::cpu::RegistersLowHigh<Index> regsLowHigh;

    SECTION("Test individual access of high/low bytes of registers.") {
        regsLowHigh.setLow(REG, 0xA);
        regsLowHigh.setHigh(REG, 0xB);

        REQUIRE(regsLowHigh.getLow(REG) == 0xA);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::LOW_BYTE) == 0xA);

        REQUIRE(regsLowHigh.getHigh(REG) == 0xB);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::HIGH_BYTE) == 0xB);

        REQUIRE(regsLowHigh.get(REG) == 0x0B0A);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::FULL_WORD) == 0x0B0A);
    }
}

TEST_CASE("Test CPU instruction representation.", "[emu][cpu][instructions]") {
    using namespace emu::cpu;

    SECTION("Test checking the direction and data size of instruction based on opcode value") {
        instr::Opcode firstOpcode(0b10);

        REQUIRE_FALSE(firstOpcode.getWordBit());
        REQUIRE(firstOpcode.getDataSize() == instr::BYTE_DATA_SIZE);

        REQUIRE(firstOpcode.getDirectionBit());
        REQUIRE(firstOpcode.getDirection() == instr::REG_IS_DESTINATION);

        instr::Opcode secondOpcode(0b01);

        REQUIRE(secondOpcode.getWordBit());
        REQUIRE(secondOpcode.getDataSize() == instr::WORD_DATA_SIZE);

        REQUIRE_FALSE(secondOpcode.getDirectionBit());
        REQUIRE(secondOpcode.getDirection() == instr::REG_IS_SOURCE);
    }
}