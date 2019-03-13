#include "emu/cpu/instr/instruction.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instructionName, Opcode instructionOpcode, std::unique_ptr<ModRegRm> modRegRm,
                             std::unique_ptr<Immediate> immediate)
    : name(instructionName),
      opcode(instructionOpcode), modRegRmByte(std::move(modRegRm)), immediateValue(std::move(immediate)) {}
}