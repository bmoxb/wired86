#include "emu/cpu/instr/complexinstruction.hpp"

#include "emu/cpu/intel8086.hpp"
#include "logging.hpp"

namespace emu::cpu::instr {
    /*
     * ComplexInstruction implementation:
     */

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

    /*
     * ComplexInstructionEG implementation:
     */

    std::string ComplexInstructionEG::argumentsToAssemblyNoDisplacement(const Intel8086& cpu,
                                                                        const assembly::Style& style) const {
        Displacement zeroDisplacement({ 0 });
        return argumentsToAssemblySpecifiedDisplacement(cpu, style, zeroDisplacement);
    }

    std::string ComplexInstructionEG::argumentsToAssemblyDisplacement(const Intel8086& cpu,
                                                                      const assembly::Style& style) const {
        if(displacementValue)
            return argumentsToAssemblySpecifiedDisplacement(cpu, style, *displacementValue);
        else {
            logging::warning("Instruction executed in displacement mode lacks a displacement value!");
            return argumentsToAssemblyNoDisplacement(cpu, style);
        }
    }

    std::string ComplexInstructionEG::argumentsToAssemblyRegisterAddressingMode(const Intel8086& cpu,
                                                                                const assembly::Style& style) const {
        DataSize size = opcode.getDataSize();

        std::string regRegisterIdentifier = modRegRm.getRegisterIdentifierFromReg(cpu.generalRegisters, size);
        std::string rmRegisterIdentifier = modRegRm.getRegisterIdentifierFromRm(cpu.generalRegisters, size);

        return argumentsToAssemblyOpcodeDirection(regRegisterIdentifier, rmRegisterIdentifier, style,
                                                  opcode.getDirection());
    }

    std::string ComplexInstructionEG::argumentsToAssemblySpecifiedDisplacement(const Intel8086& cpu,
                                                                               const assembly::Style& style,
                                                                               const Displacement& specifiedDisplacement) const {
        
        DataSize size = opcode.getDataSize();
        std::string registerIdentifier, displacementArgument;
        
        registerIdentifier = modRegRm.getRegisterIdentifierFromReg(cpu.generalRegisters, size);

        displacementArgument = specifiedDisplacement.toAssembly(size, modRegRm, cpu.generalRegisters, style);

        return argumentsToAssemblyOpcodeDirection(registerIdentifier, displacementArgument, style,
                                                  opcode.getDirection());
    }

    void ComplexInstructionEG::executeRegisterAddressingMode(Intel8086& cpu, Mem&) {
        DataSize size = opcode.getDataSize();

        auto regIndex = modRegRm.getRegisterIndexFromReg(size);
        auto regPart = modRegRm.getRegisterPartFromReg(size);
        u16 regRegisterValue = cpu.generalRegisters.get(regIndex, regPart);

        auto rmIndex = modRegRm.getRegisterIndexFromRm(size);
        auto rmPart = modRegRm.getRegisterPartFromRm(size);
        u16 rmRegisterValue = cpu.generalRegisters.get(rmIndex, rmPart);

        u16 result;

        switch(opcode.getDirection()) {
        case REG_IS_SOURCE:
            result = performOperation(rmRegisterValue, regRegisterValue);
            cpu.generalRegisters.set(rmIndex, rmPart, result);
            break;

        case REG_IS_DESTINATION:
            result = performOperation(regRegisterValue, rmRegisterValue);
            cpu.generalRegisters.set(regIndex, regPart, result);
            break;
        }
    }
}