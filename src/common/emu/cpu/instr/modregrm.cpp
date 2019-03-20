#include "emu/cpu/instr/modregrm.hpp"

#include "convert.hpp"
#include "logging.hpp"

namespace emu::cpu::instr {
    ModRegRm::ModRegRm(u8 modRegRmValue) : value(modRegRmValue) {}

    u8 ModRegRm::getRmBits() const {
        return convert::getBitsFrom(value, 0, 3);
    }

    u8 ModRegRm::getRegBits() const {
        return convert::getBitsFrom(value, 3, 3);
    }

    u8 ModRegRm::getModBits() const {
        return convert::getBitsFrom(value, 6, 2);
    }

    AddressingMode ModRegRm::getAddressingMode() const {
        u8 bits = getModBits();

        switch(bits) {
        case 0b00: return NO_DISPLACEMENT;
        case 0b01: return BYTE_DISPLACEMENT;
        case 0b10: return WORD_DISPLACEMENT;
        case 0b11: return REGISTER_ADDRESSING_MODE;
        }

        logging::warning("Invalid addressing mode specified by MOD component of MOD-REG-R/M byte: " +
                         convert::toBinaryString<8>(bits));
        return NO_DISPLACEMENT;
    }

    reg::GeneralRegister ModRegRm::getRegisterIndexFromRm(DataSize size) const {
        return getRegisterIndex(getRmBits(), size);
    }

    reg::RegisterPart ModRegRm::getRegisterPartFromRm(DataSize size) const {
        return getRegisterPart(getRmBits(), size);
    }

    reg::GeneralRegister ModRegRm::getRegisterIndexFromReg(DataSize size) const {
        return getRegisterIndex(getRegBits(), size);
    }

    reg::RegisterPart ModRegRm::getRegisterPartFromReg(DataSize size) const {
        return getRegisterPart(getRegBits(), size);
    }

    DisplacementType ModRegRm::getDisplacementType() const {
        u8 bits = getRmBits();

        switch(bits) {
        case 0b000: return BX_SI_DISPLACEMENT;
        case 0b001: return BX_DI_DISPLACEMENT;
        case 0b010: return BP_SI_DISPLACEMENT;
        case 0b011: return BP_DI_DISPLACEMENT;
        case 0b100: return SI_DISPLACEMENT;
        case 0b101: return DI_DISPLACEMENT;
        case 0b110: return BP_DISPLACEMENT;
        case 0b111: return BX_DISPLACEMENT;
        }

        logging::warning("Invalid displacement type specified by R/M component of MOD-REG-R/M byte: " +
                         convert::toBinaryString<8>(bits));
        return BX_SI_DISPLACEMENT;
    }

    reg::GeneralRegister ModRegRm::getRegisterIndex(u8 bits, DataSize size) const {
        if(size == BYTE_DATA_SIZE) {
            switch(bits) {
            case 0b000: // AL
            case 0b100: // AH
                return reg::AX_REGISTER;

            case 0b001: // CL
            case 0b101: // CH
                return reg::CX_REGISTER;

            case 0b010: // DL
            case 0b110: // DH
                return reg::DX_REGISTER;

            case 0b011: // BL
            case 0b111: // BH
                return reg::BX_REGISTER;
            }
        }

        if(size == WORD_DATA_SIZE) {
            switch(bits) {
            case 0b000: return reg::AX_REGISTER;
            case 0b001: return reg::CX_REGISTER;
            case 0b010: return reg::DX_REGISTER;
            case 0b011: return reg::BX_REGISTER;
            case 0b100: return reg::STACK_POINTER;
            case 0b101: return reg::BASE_POINTER;
            case 0b110: return reg::SOURCE_INDEX;
            case 0b111: return reg::DESTINATION_INDEX;
            }
        }

        logging::warning("Invalid register specified by component of MOD-REG-R/M byte: " +
                         convert::toBinaryString<8>(bits));
        return reg::AX_REGISTER;
    }

    reg::RegisterPart ModRegRm::getRegisterPart(u8 bits, DataSize size) const {
        if(size == BYTE_DATA_SIZE) {
            switch(bits) {
            case 0b000: // AL
            case 0b001: // CL
            case 0b010: // DL
            case 0b011: // BL
                return reg::LOW_BYTE;

            case 0b100: // AH
            case 0b101: // CH
            case 0b110: // DH
            case 0b111: // BH
                return reg::HIGH_BYTE;
            }
        }

        if(size == WORD_DATA_SIZE && bits <= 0b111) // Ensure no more than 3 bits are passed.
            return reg::FULL_WORD;

        logging::warning("Invalid register part specified by component of MOD-REG-R/M byte: " +
                         convert::toBinaryString<8>(bits));
        return reg::FULL_WORD;
    }
}