#include "emu/cpu/instr/immediate.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    Immediate::Immediate(std::vector<u8> raw) : rawData(raw) {
        if(rawData.empty()) rawData.push_back(0u); // Require the vector to have at least 1 element.
        if(rawData.size() > 2) rawData.resize(2); // Ensure vector has no more than 2 elements.
    }

    const std::vector<u8>& Immediate::getRawData() const {
        return rawData;
    }

    u16 Immediate::getValue(DataSize size) const {
        return size == WORD_DATA_SIZE ? getWordValue()
                                      : getByteValue();
    }

    u8 Immediate::getByteValue() const {
        return rawData[0];
    }

    u16 Immediate::getWordValue() const {
        return convert::createWordFromBytes(rawData[0], rawData[1]);
    }



    std::string Displacement::toAssembly(DataSize size, const ModRegRm& modRegRm, const reg::GeneralRegisters& registers,
                                         const assembly::Style& style) const {
        std::string offsetString;

        switch(modRegRm.getDisplacementType()) {
        case BX_SI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BX_REGISTER) + style.displacementAdd +
                            registers.getAssemblyIdentifier(reg::SOURCE_INDEX);
            break;

        case BX_DI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BX_REGISTER) + style.displacementAdd +
                            registers.getAssemblyIdentifier(reg::DESTINATION_INDEX);
            break;

        case BP_SI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BASE_POINTER) + style.displacementAdd +
                            registers.getAssemblyIdentifier(reg::SOURCE_INDEX);
            break;

        case BP_DI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BASE_POINTER) + style.displacementAdd +
                            registers.getAssemblyIdentifier(reg::DESTINATION_INDEX);
            break;

        case SI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::SOURCE_INDEX);
            break;

        case DI_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::DESTINATION_INDEX);
            break;

        case BP_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BASE_POINTER);
            break;

        case BX_DISPLACEMENT:
            offsetString += registers.getAssemblyIdentifier(reg::BX_REGISTER);
            break;
        }

        offsetString += style.displacementAdd + convert::toHexString(getValue(size));
        
        return style.displacementBegin + offsetString + style.displacementEnd;
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