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

    std::variant<GeneralRegister, IndexRegister> ModRegRm::getRegisterIndex(DataSize size) const {
        u8 bits = getRegBits();

        if(size == BYTE_DATA_SIZE) {
            switch(bits) {
            case 0b000: // AL
            case 0b100: // AH
                return AX_REGISTER;

            case 0b001: // CL
            case 0b101: // CH
                return CX_REGISTER;

            case 0b010: // DL
            case 0b110: // DH
                return DX_REGISTER;

            case 0b011: // BL
            case 0b111: // BH
                return BX_REGISTER;
            }
        }

        if(size == WORD_DATA_SIZE) {
            switch(bits) {
            case 0b000: return AX_REGISTER;
            case 0b001: return CX_REGISTER;
            case 0b010: return DX_REGISTER;
            case 0b011: return BX_REGISTER;
            case 0b100: return STACK_POINTER;
            case 0b101: return BASE_POINTER;
            case 0b110: return SOURCE_INDEX;
            case 0b111: return DESTINATION_INDEX;
            }
        }

        logging::warning("Invalid register specified by REG component of MOD-REG-R/M byte: " +
                         std::to_string(bits)); // TODO: Display as binary.
        return AX_REGISTER;
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

        logging::warning("Invalid addressing mode specified by R/M component of MOD-REG-R/M byte: " +
                         std::to_string(bits)); // TODO: Display bits as binary.
        return NO_DISPLACEMENT;
    }
}