#include "emu/cpu/instr/instruction.hpp"

#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode, std::unique_ptr<ModRegRm> modRegRm,
                             std::unique_ptr<Displacement> displacement, std::unique_ptr<Immediate> immediate)
    : identifier(instrIdentifier), opcode(instrOpcode), modRegRmByte(std::move(modRegRm)),
      displacementValue(std::move(displacement)), immediateValue(std::move(immediate)) {}

    OffsetAddr Instruction::execute(OffsetAddr instructionPointer, // Other args are unused and therefore are not named.
                                    Mem&, GeneralRegs&, IndexRegs&, SegmentRegs&, Flags&) {
        logging::warning("Executing instruction that has not overriden the default execution method!");

        return instructionPointer + getRawSize();
    }

    std::string Instruction::toAssembly() const {
        return identifier; // TODO: Convert to assembly properly.
    }

    std::vector<u8> Instruction::getRawData() const {
        std::vector<u8> data;

        data.push_back(opcode.value);
        if(modRegRmByte) data.push_back(modRegRmByte->value);
        if(displacementValue) convert::extendVector(data, displacementValue->rawData);
        if(immediateValue) convert::extendVector(data, immediateValue->rawData);

        return data;
    }

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
}