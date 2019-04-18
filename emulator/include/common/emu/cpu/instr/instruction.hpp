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
         * @param cpu Reference to the CPU on which this instruction is being executed.
         * @param memory Reference to the memory managed by the CPU.
         * @return The new instruction pointer value after completing execution.
         */
        virtual OffsetAddr execute(Intel8086& cpu, Mem& memory) = 0;

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         *
         * Is pure virtual and must therefore be overridden by subclasses.
         *
         * @param cpu Takes a constant reference to the CPU that this CPU was/will be executed on.
         * @return The assembly representation of this instruction.
         */
        virtual std::string toAssembly(const Intel8086& cpu) const = 0;

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

        /**
         * Convert this instruction to assembly (simply the instruction identifier followed by the register identifier).
         */
        std::string toAssembly(const Intel8086& cpu) const override;

    protected:
        const reg::GeneralRegister registerIndex;
        const reg::RegisterPart registerPart;
    };
}