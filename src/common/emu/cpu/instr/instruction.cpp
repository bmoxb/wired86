#include "emu/cpu/instr/instruction.hpp"

#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instructionName, Opcode instructionOpcode, std::unique_ptr<ModRegRm> modRegRm,
                             std::unique_ptr<Immediate> immediate)
    : name(instructionName),
      opcode(instructionOpcode), modRegRmByte(std::move(modRegRm)), immediateValue(std::move(immediate)) {}

    OffsetAddr Instruction::execute(OffsetAddr instructionPointer, // Other args are unused and therefore are not named.
                                    Mem&, GeneralRegs&, IndexRegs&, SegmentRegs&, Flags&) {
        logging::warning("Executing instruction that has not overriden the default execution method!");

        return instructionPointer + size();
    }

    std::string Instruction::toAssembly() const {
        return name + " ...";
    }

    std::vector<u8> Instruction::getRawData() const {
        std::vector<u8> data;

        data.push_back(opcode.value);
        if(modRegRmByte) data.push_back(modRegRmByte->value);
        // TODO: Add immediate and displacement bytes.

        return data;
    }

    OffsetAddr Instruction::size() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }
}