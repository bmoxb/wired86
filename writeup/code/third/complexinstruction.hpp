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

        std::string toAssembly(const Intel8086& cpu, const assembly::Style& style) const override final;

        std::vector<u8> getRawData() const override final;

    protected:
        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates no displacement addressing mode.
         */
        virtual std::string argumentsToAssemblyNoDisplacement(const Intel8086& cpu,
                                                              const assembly::Style& style) const = 0;

        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates either byte or word displacement addressing mode.
         */
        virtual std::string argumentsToAssemblyDisplacement(const Intel8086& cpu,
                                                            const assembly::Style& style) const = 0;

        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates register addressing mode.
         */
        virtual std::string argumentsToAssemblyRegisterAddressingMode(const Intel8086& cpu,
                                                                      const assembly::Style& style) const = 0;

        /**
         * Arrange two assembly instruction arguments based on the register direction value given.
         *
         * @param reg The assembly argument derrived from the REG component of the MOD-REG-R/M byte.
         * @param rm The assembly instruction argument derrived from the R/M component.
         * @param direction The direction of the instruction (usually indicated by the opcode).
         */
        std::string argumentsToAssemblyOpcodeDirection(std::string reg, std::string rm, const assembly::Style& style,
                                                       RegDirection direction) const;

        virtual void executeNoDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeByteDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeWordDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeRegisterAddressingMode(Intel8086& cpu, Mem& memory) = 0;

        const ModRegRm modRegRm;
        const std::optional<Displacement> displacementValue;
        const std::optional<Immediate> immediateValue;
    };
}
