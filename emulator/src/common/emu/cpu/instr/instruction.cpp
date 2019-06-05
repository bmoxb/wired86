#include "emu/cpu/instr/instruction.hpp"

#include <functional>
#include "emu/cpu/intel8086.hpp"
#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode)
    : identifier(instrIdentifier), opcode(instrOpcode) {}

    std::string Instruction::toAssembly(const Intel8086&) const {
        return identifier;
    }

    std::vector<u8> Instruction::getRawData() const {
        return { opcode.value }; // By default, only the opcode value is returned as raw data.
    }

    std::string Instruction::getRawDataString(std::string separator) const {
        std::vector<u8> raw = getRawData();
        std::function<std::string(u8)> convertFunction = [](u8 value) { return convert::toBinaryString<8>(value); };
        
        return convert::vectorToString(raw, convertFunction, separator);
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size(); // Fetch the raw data vector in order to then obtain its size expressed
                                                // as std::size_t type.
        return static_cast<OffsetAddr>(size); // Cast from std::size_t at compile time (static cast).
    }

    OffsetAddr Instruction::nextAddress(const Intel8086& cpu) const {
        return cpu.getRelativeInstructionPointer() + getRawSize();
    }



    InstructionTakingRegister::InstructionTakingRegister(std::string instrIdentifier, Opcode instrOpcode,
                                                         reg::GeneralRegister generalReg, reg::RegisterPart part)
    : Instruction(instrIdentifier, instrOpcode),
      registerIndex(generalReg), registerPart(part) {}

    std::string InstructionTakingRegister::toAssembly(const Intel8086& cpu) const {
        auto registerIdentifier = cpu.generalRegisters.getAssemblyIdentifier(registerIndex, registerPart);
        return identifier + " " + registerIdentifier;
    }



    HaltInstruction::HaltInstruction(Opcode instrOpcode) : Instruction("hlt", instrOpcode) {}

    OffsetAddr HaltInstruction::execute(Intel8086& cpu, Mem&) {
        cpu.halted = true;

        return nextAddress(cpu);
    }
}