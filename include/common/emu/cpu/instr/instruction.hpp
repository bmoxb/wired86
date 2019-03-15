#pragma once

#include <string>
#include <vector>
#include <memory>
#include "emu/types.hpp"
#include "emu/cpu/instr/opcode.hpp"
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/cpu/instr/immediate.hpp"

namespace emu::cpu::instr {
    class Instruction {
    public:
        /**
         * Create a new instruction representation object.
         *
         * @param instrIdentifier Assembly code identifier for this instruction (e.g. "mov").
         * @param instrOpcode The Opcode object of this instruction.
         * @param modRegRm Optional unique pointer to a MOD-REG-R/M object.
         * @param displacement Optional unique pointer to a Displacement object.
         * @param immediate Optional unique pointer to an Immediate object.
         */
        Instruction(std::string instrIdentifier, Opcode instrOpcode, std::unique_ptr<ModRegRm> modRegRm = {},
                    std::unique_ptr<Displacement> displacement = {}, std::unique_ptr<Immediate> immediate = {});

        /**
         * Execute this instruction using the given CPU internal values.
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
        virtual OffsetAddr execute(OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters, IndexRegs& indexRegisters,
                                   SegmentRegs& segmentRegisters, Flags& flags);

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         */
        std::string toAssembly() const;

        /**
         * Fetch the raw 8-bit values that make this instruction include the opcode value.
         */
        std::vector<u8> getRawData() const;

        /**
         * Fetch the raw 8-bit values that make up this instruction expressed as a string.
         */
        std::string getRawDataString(std::string separator = ", ") const;

        /**
         * Returns the number of bytes that make up this instruction.
         */
        OffsetAddr getRawSize() const;

        const std::string identifier;
        const Opcode opcode;

    protected:
        std::unique_ptr<ModRegRm> modRegRmByte;
        std::unique_ptr<Displacement> displacementValue;
        std::unique_ptr<Immediate> immediateValue;
    };
}