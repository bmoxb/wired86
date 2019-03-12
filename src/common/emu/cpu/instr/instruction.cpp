#include "emu/cpu/instr/instruction.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(Opcode instructionOpcode) : opcode(instructionOpcode) {}
}