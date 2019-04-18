#pragma once

#include <optional>
#include "emu/cpu/instr/instruction.hpp"
#include "emu/cpu/instr/modregrm.hpp"

namespace emu::cpu::instr {
    /**
     * A 'complex' instruction is one which has a MOD-REG-R/M byte and may also have a displacement and/or immediate
     * value encoded as part of the instruction.
     */
    class ComplexInstruction : public Instruction {
    public:
        ComplexInstruction(std::string instrIdentifier, Opcode instrOpcode, ModRegRm instrModRegRm,
                           std::optional<Displacement> displacement = {}, std::optional<Immediate> immediate = {});

        OffsetAddr execute(Intel8086& cpu, Mem& memory) override final;

        std::vector<u8> getRawData() const override;

    protected:
        virtual void executeNoDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeByteDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeWordDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeRegisterAddressingMode(Intel8086& cpu, Mem& memory) = 0;

        const ModRegRm modRegRm;
        const std::optional<Displacement> displacementValue;
        const std::optional<Immediate> immediateValue;
    };

    /**
     * An instruction with a MOD-REG-R/M byte taking two arguments (one of argument being either a general-purpose
     * register or a memory address and the other argument being a general-purpose register).
     *
     * Examples:
     * - ADD (0x00, 0x01, 0x02, 0x03)
     * - XOR (0x30, 0x31, 0x32, 0x33)
     */
    class InstructionEG : public ComplexInstruction {
    public:
        using ComplexInstruction::ComplexInstruction;
    };
}