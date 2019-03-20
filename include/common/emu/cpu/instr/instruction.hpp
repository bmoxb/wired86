#pragma once

#include <string>
#include <vector>
#include "emu/types.hpp"
#include "emu/cpu/instr/opcode.hpp"
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/cpu/instr/immediate.hpp"

namespace emu::cpu { class Intel8086; } // Declared as incomplete type as including the CPU header here would cause a
                                        // circular dependency.

namespace emu::cpu::instr {
    class Instruction {
    public:
        /**
         * Create a new instruction representation object.
         *
         * @param instrIdentifier Assembly code identifier for this instruction (e.g. "mov").
         * @param instrOpcode The Opcode object of this instruction.
         */
        Instruction(std::string instrIdentifier, Opcode instrOpcode);

        /**
         * Execute this instruction using the given CPU internal values.
         *
         * Is pure virtual and must therefore be overridden by subclasses.
         *
         * It is the role of this method to return the appropriate instruction pointer value so that the correct next
         * instruction is executed. Unless a jumping/calling instruction, in most instances it is best to return the
         * current instruction pointer plus the value returned by Instruction::getRawSize.
         *
         * @param ip The current value of the instruction pointer.
         * @param memory Reference to the memory managed by the CPU.
         * @param generalRegisters Reference to general-purpose registers of the CPU this instruction is executed on.
         * @param indexRegisters Reference to the CPU's indexing registers.
         * @param segmentRegisters Reference to the CPU's segment registers.
         * @param flags Reference to the CPU's status flags.
         * @return The new instruction pointer value after completing execution.
         */
        virtual OffsetAddr execute(Intel8086& cpu, OffsetAddr ip, Mem& memory, reg::GeneralRegisters& generalRegisters,
                                   reg::SegmentRegisters& segmentRegisters, reg::Flags& flags) = 0;

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         *
         * Is pure virtual and must therefore be overridden by subclasses.
         */
        virtual std::string toAssembly(const reg::GeneralRegisters& generalRegisters,
                                       const reg::SegmentRegisters& segmentRegisters,
                                       const reg::Flags& flags) const = 0;

        /**
         * Fetch the raw 8-bit values that make this instruction include the opcode value.
         *
         * By default, only returns the opcode raw value. More complex instructions that take a MOD-REG-R/M byte,
         * displacement value or immediate value should override this method.
         */
        virtual std::vector<u8> getRawData() const;

        /**
         * Fetch the raw 8-bit values that make up this instruction expressed as a string.
         *
         * @param separator The separating string placed between each binary value in the string representation.
         */
        std::string getRawDataString(std::string separator = ", ") const;

        /**
         * Returns the number of bytes that make up this instruction.
         */
        OffsetAddr getRawSize() const;

        const std::string identifier;
        const Opcode opcode;
    };

    /**
     * For representing instructions taking a single specific register that is not specified by a MOD-REG-R/M byte.
     *
     * Examples:
     * - PUSH ES (0x06)
     * - INC AX (0x40)
     * - PUSH BP (0x55)
     */
    class InstructionTakingRegister : public Instruction {
    public:
        /**
         * @param generalReg The register index that this instruction takes as an argument.
         * @param part The register part used by this instruction (defaults to using the full 16-bit register).
         */
        InstructionTakingRegister(std::string instrIdentifier, Opcode instrOpcode, reg::GeneralRegister generalReg,
                                  reg::RegisterPart part = reg::FULL_WORD);

        std::string toAssembly(const reg::GeneralRegisters& generalRegisters, const reg::SegmentRegisters&,
                               const reg::Flags&) const override;

    protected:
        const reg::GeneralRegister registerIndex;
        const reg::RegisterPart registerPart;
    };

    /**
     * Instructions that take a general-purpose register as an argument and either a general-purpose register or a
     * memory address as its other argument.
     *
     * Examples:
     * - ADD Eb Gb (0x00)
     * - MOV Eb Gb (0x88)
     */
    class InstructionTakingRegAndRegOrAddr : public Instruction {
    public:
        InstructionTakingRegAndRegOrAddr(std::string instrIdentifier, Opcode instrOpcode, ModRegRm modRegRm,
                                         std::optional<Displacement> displacement = {});

        //OffsetAddr execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
        //                   GeneralRegs& generalRegisters, SegmentRegs& segmentRegisters, Flags& flags) override;

        //std::string toAssembly() const override;

    protected:
        virtual void handleExecution(reg::GeneralRegister destination, reg::GeneralRegister source) = 0;
        virtual void handleExecution(reg::GeneralRegister destination, AbsAddr source) = 0;
        virtual void handleExecution(AbsAddr destination, reg::GeneralRegister source) = 0;

    private:
        const ModRegRm modRegRmByte;
        const std::optional<Displacement> displacementValue;
    };
}