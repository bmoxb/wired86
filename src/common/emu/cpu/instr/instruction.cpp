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



    InstructionGE::InstructionGE(std::string instrIdentifier, Opcode instrOpcode, ModRegRm modRegRm)
    : Instruction(instrIdentifier, instrOpcode), modRegRmByte(modRegRm) {}

    OffsetAddr InstructionGE::execute(OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters, SegmentRegs&,
                                                Flags&) {
        auto dataSize = opcode.getDataSize();
        auto addressingMode = modRegRmByte.getAddressingMode();
        auto reg = modRegRmByte.getRegisterIndexFromReg(dataSize);

        if(addressingMode == REGISTER_ADDRESSING_MODE) {
            auto rm = modRegRmByte.getRegisterIndexFromRm(dataSize);
            callHandlerWithCorrectOrdering(reg, rm, memory, generalRegisters);
        }
        else if(displacement) { // Using an address as an argument instead of an additional register.
            auto displacementType = modRegRmByte.getDisplacementType();
            AbsAddr address = displacement->resolve(addressingMode, displacementType, generalRegisters);
            u8 memoryValue = memory.read(address);

            callHandlerWithCorrectOrdering(reg, memoryValue, memory, generalRegisters);
        }

        return ip + getRawSize();
    }

    std::vector<u8> InstructionGE::getRawData() const {
        std::vector<u8> raw = { opcode.value, modRegRmByte.value };
        if(displacement) convert::extendVector(raw, displacement->rawData);
        return raw;
    }
}