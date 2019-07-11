#include "emu/cpu/instr/immediate.hpp"

#include <cassert>
#include "convert.hpp"

namespace emu::cpu::instr {
    Immediate::Immediate(std::vector<u8> raw) : rawData(raw) {
        assert(rawData.size() >= 1);
    }

    const std::vector<u8>& Immediate::getRawData() const {
        return rawData;
    }

    u8 Immediate::getByteValue() const {
        return rawData[0];
    }

    u16 Immediate::getWordValue() const {
        assert(rawData.size() >= 2);
        return convert::createWordFromBytes(rawData[0], rawData[1]);
    }

    AbsAddr Displacement::resolve(AddressingMode mode, DisplacementType type, reg::GeneralRegisters& registers) const {
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
            return registers.get(reg::BX_REGISTER) +
                   registers.get(reg::SOURCE_INDEX) +
                   displacementValue;

        case BX_DI_DISPLACEMENT:
            return registers.get(reg::BX_REGISTER) +
                   registers.get(reg::DESTINATION_INDEX) +
                   displacementValue;

        case BP_SI_DISPLACEMENT:
            return registers.get(reg::BASE_POINTER) +
                   registers.get(reg::SOURCE_INDEX) +
                   displacementValue;

        case BP_DI_DISPLACEMENT:
            return registers.get(reg::BASE_POINTER) +
                   registers.get(reg::DESTINATION_INDEX) +
                   displacementValue;

        case SI_DISPLACEMENT:
            return registers.get(reg::SOURCE_INDEX) + displacementValue;

        case DI_DISPLACEMENT:
            return registers.get(reg::DESTINATION_INDEX) + displacementValue;

        case BP_DISPLACEMENT:
            return registers.get(reg::BASE_POINTER) + displacementValue;

        default: // BX_DISPLACEMENT
            return registers.get(reg::BX_REGISTER) + displacementValue;
        }
    }
}