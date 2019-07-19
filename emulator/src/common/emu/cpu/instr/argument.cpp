#include "emu/cpu/instr/argument.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    /*
     * DataArgument implementation:
     */

    DataArgument::DataArgument(std::vector<u8> raw) : rawData(raw) {
        if(rawData.empty()) rawData.push_back(0u); // Require the vector to have at least 1 element.
        if(rawData.size() > 2) rawData.resize(2); // Ensure vector has no more than 2 elements.
    }

    const std::vector<u8>& DataArgument::getRawData() const {
        return rawData;
    }

    u16 DataArgument::getValueUsingDataSize(DataSize size) const {
        if(size == WORD_DATA_SIZE) return getWordValue();
        else return static_cast<u16>(getByteValue());
    }

    u8 DataArgument::getByteValue() const {
        return rawData[0];
    }

    u16 DataArgument::getWordValue() const {
        u8 low = rawData[0];
        u8 high = rawData.size() > 1 ? rawData[1] : 0; // Get rawData[1] if present. Otherwise, assume high byte is 0.

        return convert::createWordFromBytes(low, high);
    }

    /*
     * Immediate implementation:
     */

    std::string Immediate::toAssembly(DataSize size, const ModRegRm&, const reg::GeneralRegisters&,
                                         const assembly::Style& style) const {
        u16 value = getValueUsingDataSize(size);

        return convert::numberToAssembly(value, style);
    }

    /*
     * Displacement implementation:
     */

    std::string Displacement::toAssembly(DataSize, const ModRegRm& modRegRm,
                                         const reg::GeneralRegisters& registers, const assembly::Style& style) const {
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

        u16 displacementValue = getValueUsingAddressingMode(modRegRm.getAddressingMode());

        // Only add displacement value assembly if there actually is a displacement value stored:
        if(displacementValue != 0)
            offsetString += style.displacementAdd + convert::numberToAssembly(displacementValue, style);

        return style.displacementBegin + offsetString + style.displacementEnd;
    }

    u16 Displacement::getValueUsingAddressingMode(AddressingMode mode) const {
        switch(mode) {
        case BYTE_DISPLACEMENT:
            return static_cast<u16>(getByteValue());

        case WORD_DISPLACEMENT:
            return getWordValue();

        default: return 0; // Invalid addressing mode so just return 0.
        }
    }

    AbsAddr Displacement::resolve(AddressingMode mode, DisplacementType type, reg::GeneralRegisters& registers) const {
        u16 displacementValue = getValueUsingAddressingMode(mode);

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