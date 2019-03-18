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
        std::function<std::string(u8)> convert = [](u8 value) { return convert::toBinaryString(value); };
        
        return convert::vectorToString(raw, convert, separator);
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }



    InstructionTakingRegister::InstructionTakingRegister(std::string instrIdentifier, Opcode instrOpcode,
                                                         GeneralRegister reg, RegisterPart part)
    : Instruction(instrIdentifier, instrOpcode),
      registerIndex(reg), registerPart(part) {}

    std::string InstructionTakingRegister::toAssembly() const {
        return identifier + " ..."; // TODO: Get string identifier for the register.
    }



    InstructionTakingRegAndRegOrAddr::InstructionTakingRegAndRegOrAddr(std::string instrIdentifier, Opcode instrOpcode,
                                                                       ModRegRm modRegRm,
                                                                       std::optional<Displacement> displacement)
    : Instruction(instrIdentifier, instrOpcode),
      modRegRmByte(modRegRm), displacementValue(displacement) {}
}