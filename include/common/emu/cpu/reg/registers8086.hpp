#pragma once

#include "primitives.hpp"
#include "emu/cpu/reg/registers.hpp"

namespace emu::cpu::reg {
    enum GeneralRegister {
        AX_REGISTER,
        BX_REGISTER,
        CX_REGISTER,
        DX_REGISTER,
        SOURCE_INDEX,
        DESTINATION_INDEX,
        BASE_POINTER,
        STACK_POINTER
    };

    class GeneralRegisters : public RegistersLowHigh<GeneralRegister> {
    public:
        std::string getAssemblyIdentifier(GeneralRegister index) const override final;
        std::string getAssemblyIdentifier(GeneralRegister index, RegisterPart part) const override final;
    };

    enum SegmentRegister {
        CODE_SEGMENT,
        DATA_SEGMENT,
        EXTRA_SEGMENT,
        STACK_SEGMENT
    };

    class SegmentRegisters : public Registers<SegmentRegister, u16> {
    public:
        std::string getAssemblyIdentifier(SegmentRegister index) const override final;
    };

    enum Flag {
        CARRY_FLAG,
        PARITY_FLAG,
        AUX_CARRY_FLAG,
        ZERO_FLAG,
        SIGN_FLAG,
        TRAP_FLAG,
        INTERRUPT_FLAG,
        DIRECTION_FLAG,
        OVERFLOW_FLAG
    };

    class Flags : public Registers<Flag, bool> {
    public:
        std::string getAssemblyIdentifier(Flag) const override final;
    };
}