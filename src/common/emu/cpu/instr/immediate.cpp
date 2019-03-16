#include "emu/cpu/instr/immediate.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    Immediate::Immediate(std::vector<u8> raw) : rawData(raw) {} // TODO: Check length of raw.

    u8 Immediate::getByteValue() const {
        return rawData[0];
    }

    u16 Immediate::getWordValue() const {
        return convert::createWordFromBytes(rawData[0], rawData[1]);
    }

    AbsAddr Displacement::resolve(AddressingMode mode, DisplacementType type, GeneralRegs& registers) const {
        u16 displacementValue;

        switch(mode) {
        case BYTE_DISPLACEMENT:
            displacementValue = static_cast<u16>(getByteValue()); break;

        case WORD_DISPLACEMENT:
            displacementValue = getWordValue(); break;

        default: // No displacement.
            displacementValue = 0;
        }

        switch(type) {
        case BX_SI_DISPLACEMENT:
            return registers.get(BX_REGISTER) +
                   registers.get(SOURCE_INDEX) +
                   displacementValue;

        case BX_DI_DISPLACEMENT:
            return registers.get(BX_REGISTER) +
                   registers.get(DESTINATION_INDEX) +
                   displacementValue;

        case BP_SI_DISPLACEMENT:
            return registers.get(BASE_POINTER) +
                   registers.get(SOURCE_INDEX) +
                   displacementValue;

        case BP_DI_DISPLACEMENT:
            return registers.get(BASE_POINTER) +
                   registers.get(DESTINATION_INDEX) +
                   displacementValue;

        case SI_DISPLACEMENT:
            return registers.get(SOURCE_INDEX) + displacementValue;

        case DI_DISPLACEMENT:
            return registers.get(DESTINATION_INDEX) + displacementValue;

        case BP_DISPLACEMENT:
            return registers.get(BASE_POINTER) + displacementValue;

        default: // BX_DISPLACEMENT
            return registers.get(BX_REGISTER) + displacementValue;
        }
    }
}