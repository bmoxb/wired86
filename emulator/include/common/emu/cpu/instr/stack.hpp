#pragma once

#include "emu/cpu/instr/instruction.hpp"

namespace emu::cpu::instr {
    /**
     * Push instruction taking a single argument.
     */
    class PushTakingRegister : public InstructionTakingRegister {
    public:
        PushTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg);

        OffsetAddr execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
                           reg::GeneralRegisters& generalRegisters, reg::SegmentRegisters&, reg::Flags&) override final;
    };

    /**
     * Pop instruction taking a single argument.
     */
    class PopTakingRegister : public InstructionTakingRegister {
    public:
        PopTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg);

        OffsetAddr execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
                           reg::GeneralRegisters& generalRegisters, reg::SegmentRegisters&, reg::Flags&) override final;
    };
}