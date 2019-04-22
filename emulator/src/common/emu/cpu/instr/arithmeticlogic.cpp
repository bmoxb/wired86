#include "emu/cpu/instr/arithmeticlogic.hpp"

namespace emu::cpu::instr {
    AddEG::AddEG(Opcode instrOpcode, ModRegRm instrModRegRm,
                 std::optional<Displacement> displacement, std::optional<Immediate> immediate)
    : ComplexInstructionEG("add", instrOpcode, instrModRegRm, displacement, immediate) {}

    u16 AddEG::performOperation(u16 dest, u16 src) {
        return dest + src;
    }
}