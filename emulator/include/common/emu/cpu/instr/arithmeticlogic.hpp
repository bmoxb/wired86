#pragma once

#include "emu/cpu/instr/complexinstruction.hpp"

namespace emu::cpu::instr {
    class AddEG : public ComplexInstructionEG {
    public:
        AddEG(Opcode instrOpcode, ModRegRm instrModRegRm,
              std::optional<Displacement> displacement = {}, std::optional<Immediate> immediate = {});

    protected:
        u16 performOperation(u16 dest, u16 src) override final;
    };
}