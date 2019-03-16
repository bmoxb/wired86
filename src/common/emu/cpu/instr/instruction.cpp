#include "emu/cpu/instr/instruction.hpp"

#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode)
    : identifier(instrIdentifier), opcode(instrOpcode) {}

    std::string Instruction::getRawDataString(std::string separator) const {
        auto raw = getRawData();
        std::string str;

        for(unsigned int i = 0; i < raw.size() - 1; i++)
            str += convert::toHexString(raw[i]) + separator; // Add all elements (except the final one) with separator.
        
        str += convert::toHexString(raw[raw.size() - 1]); // Add final element (no separator).

        return str;
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }



    InstructionWithModRegRm::InstructionWithModRegRm(std::string instrIdentifier, Opcode instrOpcode, ModRegRm modRegRm)
    : Instruction(instrIdentifier, instrOpcode), modRegRmByte(modRegRm) {}

    OffsetAddr InstructionWithModRegRm::execute(OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters, SegmentRegs&,
                                                Flags&) {
        auto dataSize = opcode.getDataSize();
        auto addressingMode = modRegRmByte.getAddressingMode();
        auto reg = modRegRmByte.getRegisterIndexFromReg(dataSize);

        if(addressingMode == REGISTER_ADDRESSING_MODE) {
            auto rm = modRegRmByte.getRegisterIndexFromRm(dataSize);
            callHandlerWithCorrectOrdering(reg, rm, memory, generalRegisters);
        }
        else if(displacement) { // Using an address as an argument instead of an additional register.
            u16 displacementValue;

            switch(addressingMode) {
            case BYTE_DISPLACEMENT:
                displacementValue = static_cast<u16>(displacement->getByteValue());
                break;
            
            case WORD_DISPLACEMENT:
                displacementValue = displacement->getWordValue();
                break;

            default: // No displacement.
                displacementValue = 0;
            }

            AbsAddr address = resolveDisplacementToAddress(displacementValue, generalRegisters);
            u8 memoryValue = memory.read(address);

            callHandlerWithCorrectOrdering(reg, memoryValue, memory, generalRegisters);
        }

        return ip + getRawSize();
    }

    std::vector<u8> InstructionWithModRegRm::getRawData() const {
        std::vector<u8> raw = { opcode.value, modRegRmByte.value };
        if(displacement) convert::extendVector(raw, displacement->rawData);
        return raw;
    }

    AbsAddr InstructionWithModRegRm::resolveDisplacementToAddress(u16 displacementValue, GeneralRegs& registers) const {
        switch(modRegRmByte.getDisplacementType()) {
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

        default:
            return registers.get(BX_REGISTER) + displacementValue;
        }
    }
}