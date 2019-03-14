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
        Instruction(std::string instructionName, Opcode instructionOpcode, std::unique_ptr<ModRegRm> modRegRm = {},
                    std::unique_ptr<Immediate> immediate = {});

        /**
         * Execute this instruction. It is the role of this method to return the appropriate instruction pointer value
         * for the next instruction.
         *
         * @param ip The current value of the instruction pointer.
         * @param memory Reference to the memory managed by the CPU.
         * @param generalRegisters Reference to general-purpose registers of the CPU this instruction is executed on.
         * @param indexRegisters Reference to the CPU's indexing registers.
         * @param segmentRegisters Reference to the CPU's segment registers.
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
        OffsetAddr size() const;

        const std::string name;
        const Opcode opcode;

    protected:
        std::unique_ptr<ModRegRm> modRegRmByte;
        //std::unique_ptr<Displacement> displacementValue;
        std::unique_ptr<Immediate> immediateValue;
    };
}