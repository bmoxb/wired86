#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <memory>
#include "primitives.hpp"
#include "convert.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/registers.hpp"
#include "emu/cpu/instr/instruction.hpp"

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

    SECTION("Test conversion between a numerical value and a hexadecimal string representation.") {
        REQUIRE(toHexString<u8>(0xFF) == "0xFF");
        REQUIRE(toHexString<u16>(0x55A, "", "h") == "55Ah");
    }

    SECTION("Test conversion between a numerical value a binary string representation.") {
        REQUIRE(toBinaryString<u8, 8>(0b10101010) == "0b10101010");
        REQUIRE(toBinaryString<u16, 4>(0xFF, "", "b") == "1111b");
    }

    SECTION("Test the extension of vectors.") {
        std::vector<u16> vec = { 0xA, 0xB },
                         extendBy = { 0xC, 0xD, 0xE },
                         expected = { 0xA, 0xB, 0xC, 0xD, 0xE };
        
        extendVector<u16>(vec, extendBy);
        REQUIRE(vec == expected);
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

    SECTION("Test checking the direction and data size of instruction based on opcode value.") {
        instr::Opcode firstOpcode(0b10101010);

        REQUIRE(firstOpcode.getUniqueValue() == 0b101010);

        REQUIRE_FALSE(firstOpcode.getWordBit());
        REQUIRE(firstOpcode.getDataSize() == instr::BYTE_DATA_SIZE);

        REQUIRE(firstOpcode.getDirectionBit());
        REQUIRE(firstOpcode.getDirection() == instr::REG_IS_DESTINATION);

        instr::Opcode secondOpcode(0b1010101);

        REQUIRE(secondOpcode.getUniqueValue() == 0b10101);

        REQUIRE(secondOpcode.getWordBit());
        REQUIRE(secondOpcode.getDataSize() == instr::WORD_DATA_SIZE);

        REQUIRE_FALSE(secondOpcode.getDirectionBit());
        REQUIRE(secondOpcode.getDirection() == instr::REG_IS_SOURCE);
    }

    SECTION("Test MOD-REG-R/M byte representation.") {
        SECTION("Test MOD component and fetching of addressing mode.") {
            instr::ModRegRm noDisplace(0b00110011);
            REQUIRE(noDisplace.getModBits() == 0b00);
            REQUIRE(noDisplace.getAddressingMode() == instr::NO_DISPLACEMENT);

            instr::ModRegRm byteDisplace(0b01010101);
            REQUIRE(byteDisplace.getModBits() == 0b01);
            REQUIRE(byteDisplace.getAddressingMode() == instr::BYTE_DISPLACEMENT);

            instr::ModRegRm wordDisplace(0b10101010);
            REQUIRE(wordDisplace.getModBits() == 0b10);
            REQUIRE(wordDisplace.getAddressingMode() == instr::WORD_DISPLACEMENT);

            instr::ModRegRm regAddressing(0b11001100);
            REQUIRE(regAddressing.getModBits() == 0b11);
            REQUIRE(regAddressing.getAddressingMode() == instr::REGISTER_ADDRESSING_MODE);
        }

        SECTION("Test REG component.") {
            instr::ModRegRm axReg(0b11000101);
            REQUIRE(axReg.getRegBits() == 0b000);
            REQUIRE(axReg.getRegisterIndexFromReg(instr::WORD_DATA_SIZE) == AX_REGISTER);
            REQUIRE(axReg.getRegisterPartFromReg(instr::WORD_DATA_SIZE) == FULL_WORD);

            instr::ModRegRm clReg(0b11001010);
            REQUIRE(clReg.getRegBits() == 0b001);
            REQUIRE(clReg.getRegisterIndexFromReg(instr::BYTE_DATA_SIZE) == CX_REGISTER);
            REQUIRE(axReg.getRegisterPartFromReg(instr::BYTE_DATA_SIZE) == LOW_BYTE);

            instr::ModRegRm dhReg(0b11110000);
            REQUIRE(dhReg.getRegBits() == 0b110);
            REQUIRE(dhReg.getRegisterIndexFromReg(instr::BYTE_DATA_SIZE) == DX_REGISTER);
            REQUIRE(dhReg.getRegisterPartFromReg(instr::BYTE_DATA_SIZE) == HIGH_BYTE);

            instr::ModRegRm diReg(0b11111001);
            REQUIRE(diReg.getRegBits() == 0b111);
            REQUIRE(diReg.getRegisterIndexFromReg(instr::WORD_DATA_SIZE) == DESTINATION_INDEX);
            REQUIRE(diReg.getRegisterPartFromReg(instr::WORD_DATA_SIZE) == FULL_WORD);
        }

        SECTION("Test the fetching of displacement types via R/M component.") {
            REQUIRE(instr::ModRegRm(0b10000000).getDisplacementType() == instr::BX_SI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b10111001).getDisplacementType() == instr::BX_DI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b01001010).getDisplacementType() == instr::BP_SI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b00101011).getDisplacementType() == instr::BP_DI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b10111100).getDisplacementType() == instr::SI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b01000101).getDisplacementType() == instr::DI_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b00101110).getDisplacementType() == instr::BP_DISPLACEMENT);
            REQUIRE(instr::ModRegRm(0b10111111).getDisplacementType() == instr::BX_DISPLACEMENT);
        }
    }

    SECTION("Test immediate instruction value representation.") {
        std::vector<u8> firstData = { 0xAA, 0xBB };
        instr::Immediate first(firstData);
        
        REQUIRE(first.rawData == firstData);
        REQUIRE(first.getByteValue() == 0xAA);
        REQUIRE(first.getWordValue() == 0xBBAA);
    }
}