#include "emu/cpu/instr/complexinstruction.hpp"

#include "emu/cpu/intel8086.hpp"

namespace emu::cpu::instr {
    ComplexInstruction::ComplexInstruction(std::string instrIdentifier, Opcode instrOpcode, ModRegRm instrModRegRm,
                                           std::optional<Displacement> displacement, std::optional<Immediate> immediate)
    : Instruction(instrIdentifier, instrOpcode), modRegRm(instrModRegRm),
      displacementValue(displacement), immediateValue(immediate) {}

    OffsetAddr ComplexInstruction::execute(Intel8086& cpu, Mem& memory) {
        switch(modRegRm.getAddressingMode()) {
        case NO_DISPLACEMENT:
            executeNoDisplacement(cpu, memory); break;

        case BYTE_DISPLACEMENT:
            executeByteDisplacement(cpu, memory); break;

        case WORD_DISPLACEMENT:
            executeWordDisplacement(cpu, memory); break;

        case REGISTER_ADDRESSING_MODE:
            executeRegisterAddressingMode(cpu, memory); break;
        }

        return cpu.getRelativeInstructionPointer() + getRawSize();
    }

    std::vector<u8> ComplexInstruction::getRawData() const {
        std::vector<u8> data = { opcode.value, modRegRm.value };

        if(displacementValue) convert::extendVector(data, displacementValue->rawData);
        if(immediateValue) convert::extendVector(data, immediateValue->rawData);

        return data;
    }
}