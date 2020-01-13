#include "emu/cpu/instr/complexinstruction.hpp"

#include "emu/cpu/intel8086.hpp"
#include "logging.hpp"

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

        return nextAddress(cpu);
    }

    std::string ComplexInstruction::toAssembly(const Intel8086& cpu, const assembly::Style& style) const {
        std::string argumentsStr;

        switch(modRegRm.getAddressingMode()) {
        case NO_DISPLACEMENT:
            argumentsStr = argumentsToAssemblyNoDisplacement(cpu, style); break;

        case REGISTER_ADDRESSING_MODE:
            argumentsStr = argumentsToAssemblyRegisterAddressingMode(cpu, style); break;

        default: // Byte or word displacement:
            argumentsStr = argumentsToAssemblyDisplacement(cpu, style); break;
        }

        return identifier + " " + argumentsStr;
    }

    std::vector<u8> ComplexInstruction::getRawData() const {
        std::vector<u8> data = { opcode.value, modRegRm.value };

        if(displacementValue) convert::extendVector(data, displacementValue->getRawData());
        if(immediateValue) convert::extendVector(data, immediateValue->getRawData());

        return data;
    }

    std::string ComplexInstruction::argumentsToAssemblyOpcodeDirection(std::string reg, std::string rm,
                                                                       const assembly::Style& style,
                                                                       RegDirection direction) const {
        std::string arguments = "";

        switch(direction) {
        case REG_IS_SOURCE:
            arguments = rm + style.argumentSeparator + reg; break;

        case REG_IS_DESTINATION:
            arguments = reg + style.argumentSeparator + rm; break;
        }

        return arguments;
    }
}
