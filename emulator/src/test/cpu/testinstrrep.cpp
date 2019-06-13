#include "catch.hpp"
#include "emu/cpu/intel8086.hpp"

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
            REQUIRE(axReg.getRegisterIndexFromReg(instr::WORD_DATA_SIZE) == reg::AX_REGISTER);
            REQUIRE(axReg.getRegisterPartFromReg(instr::WORD_DATA_SIZE) == reg::FULL_WORD);

            instr::ModRegRm clReg(0b11001010);
            REQUIRE(clReg.getRegBits() == 0b001);
            REQUIRE(clReg.getRegisterIndexFromReg(instr::BYTE_DATA_SIZE) == reg::CX_REGISTER);
            REQUIRE(clReg.getRegisterPartFromReg(instr::BYTE_DATA_SIZE) == reg::LOW_BYTE);

            instr::ModRegRm dhReg(0b11110000);
            REQUIRE(dhReg.getRegBits() == 0b110);
            REQUIRE(dhReg.getRegisterIndexFromReg(instr::BYTE_DATA_SIZE) == reg::DX_REGISTER);
            REQUIRE(dhReg.getRegisterPartFromReg(instr::BYTE_DATA_SIZE) == reg::HIGH_BYTE);

            instr::ModRegRm diReg(0b11111001);
            REQUIRE(diReg.getRegBits() == 0b111);
            REQUIRE(diReg.getRegisterIndexFromReg(instr::WORD_DATA_SIZE) == reg::DESTINATION_INDEX);
            REQUIRE(diReg.getRegisterPartFromReg(instr::WORD_DATA_SIZE) == reg::FULL_WORD);
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