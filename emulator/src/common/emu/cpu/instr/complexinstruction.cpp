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

        return nextAddress(cpu);
    }

    std::vector<u8> ComplexInstruction::getRawData() const {
        std::vector<u8> data = { opcode.value, modRegRm.value };

        if(displacementValue) convert::extendVector(data, displacementValue->rawData);
        if(immediateValue) convert::extendVector(data, immediateValue->rawData);

        return data;
    }



    std::string ComplexInstructionEG::toAssembly(const Intel8086&) const {
        return identifier + " ..."; // TODO: Implement!
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