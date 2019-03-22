#include "emu/cpu/instr/instruction.hpp"

#include <functional>
#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode)
    : identifier(instrIdentifier), opcode(instrOpcode) {}

    std::vector<u8> Instruction::getRawData() const {
        return { opcode.value }; // By default, only the opcode value is returned as raw data.
    }

    std::string Instruction::getRawDataString(std::string separator) const {
        std::vector<u8> raw = getRawData();
        std::function<std::string(u8)> convertFunction = [](u8 value) { return convert::toBinaryString<8>(value); };
        
        return convert::vectorToString(raw, convertFunction, separator);
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }



    InstructionTakingRegister::InstructionTakingRegister(std::string instrIdentifier, Opcode instrOpcode,
                                                         reg::GeneralRegister generalReg, reg::RegisterPart part)
    : Instruction(instrIdentifier, instrOpcode),
      registerIndex(generalReg), registerPart(part) {}

    std::string InstructionTakingRegister::toAssembly(const reg::GeneralRegisters& generalRegisters,
                                                      const reg::SegmentRegisters&, const reg::Flags&) const {
        return identifier + " " + getRegisterAssembly(generalRegisters);
    }

    std::string InstructionTakingRegister::getRegisterAssembly(const reg::GeneralRegisters& registers) const {
        return registers.getAssemblyIdentifier(registerIndex, registerPart);
    }
}