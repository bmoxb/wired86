#pragma once

#include "types.hpp"

namespace emu::cpu {
    /**
     * Class representing the main Intel 8086 microprocessor. Handles decoding and execution of instructions fetched
     * from memory.
     */
    class Intel8086 {
    public:
        /**
         * Takes a 16-bit memory offset and a 16-bit segment register and returns an absolute 20-bit address (which is
         * stored in an unsigned 32-bit integer since there is no 20-bit integer type available in C++).
         *
         * @param offset The memory offset within the given segment.
         * @param segment Segment register index indicating which segment to resolve the offset within.
         * @return Absolute 20-bit address within memory.
         */
        AbsAddr resolveAddress(OffsetAddr offset, SegmentRegister segment) const;

        /**
         * Calculate the address of the next instruction in memory based on the value of the instruction pointer and the
         * code segment.
         *
         * @return Address of next instruction.
         */
        AbsAddr nextInstructionAddress() const;

        /**
         * Fetches and decodes the next instruction.
         *
         * @param addr The absolute address of the instruction to fetch and decode.
         * @param memory Reference to the memory to fetch the instruction data from.
         * @return Decoded instruction object.
         */
        InstructionPtr fetchDecodeInstruction(AbsAddr address, const Mem& memory) const;

        /**
         * Executes a decoded instruction on this CPU.
         *
         * @param instruction Reference to the std::unique_ptr holding the instruction.
         */
        void executeInstruction(InstructionPtr& instruction, const Mem& memory);

    private:
        /// The instruction pointer is an offset within the code segment that points to the next instruction in memory.
        OffsetAddr instructionPointer = 0;

        GeneralRegs generalRegisters;
        IndexRegs indexRegisters;
        SegmentRegs segmentRegisters;
        Flags flags;
    };
}