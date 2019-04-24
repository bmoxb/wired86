#pragma once

#include <memory>
#include "emu/types.hpp"
#include "emu/cpu/instr/instruction.hpp"
#include "emu/cpu/reg/registers8086.hpp"

namespace emu::cpu {
    /**
     * Class representing the main Intel 8086 microprocessor. Handles decoding and execution of instructions fetched
     * from memory. Also holds all CPU registers.
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
        AbsAddr resolveAddress(OffsetAddr offset, reg::SegmentRegister segment) const;

        /**
         * Returns the relative address of the instruction pointer (i.e. before it is segmented within the code
         * segment).
         */
        OffsetAddr getRelativeInstructionPointer() const;

        /**
         * Calculate the address of the next instruction in memory based on the value of the instruction pointer and the
         * code segment. Does not increment instruction pointer - that is done on execution.
         *
         * @return Address of next instruction.
         */
        AbsAddr getAbsoluteInstructionPointer() const;

        /**
         * Fetches and decodes the next instruction.
         *
         * @param address The absolute address of the instruction to fetch and decode.
         * @param memory Reference to the memory to fetch the instruction data from.
         * @return Decoded instruction object (will be empty if instruction decoding fails).
         */
        std::unique_ptr<instr::Instruction> fetchDecodeInstruction(AbsAddr address, const Mem& memory) const;

        /**
         * Executes a decoded instruction on this CPU.
         *
         * @param instruction Reference to the std::unique_ptr holding the instruction.
         */
        void executeInstruction(std::unique_ptr<instr::Instruction>& instruction, Mem& memory);

        /**
         * Push values onto the stack. Stack pointer decremented.
         */
        void pushToStack(MemValue value, Mem& memory);

        /**
         * Pop values from the stack. Stack pointer incremented.
         */
        MemValue popFromStack(Mem& memory);

        /**
         * Push a 16-bit word value onto the stack.
         */
        void pushWordToStack(u16 value, Mem& memory);

        /**
         * Pop 16-bit word value off the stack.
         */
        u16 popWordFromStack(Mem& memory);

        /**
         * Sets the instruction pointer without altering any segment addresses.
         */
        void performRelativeJump(OffsetAddr offset);

        reg::GeneralRegisters generalRegisters; /// CPU general-purpose registers.
        reg::SegmentRegisters segmentRegisters; /// CPU segment registers.

    private:
        /**
         * @param opcode The instruction opcode.
         * @return The decoded instruction or an empty unique pointer if decoding failed.
         */
        std::unique_ptr<instr::Instruction> fetchDecodeWithoutModRegRm(const instr::Opcode& opcode) const;

        /**
         * @param opcode The instruction opcode.
         * @param modRegRm The instruction MOD-REG-R/M byte.
         * @return The coded instruction or an empty unique pointer if decoding failed.
         */
        std::unique_ptr<instr::Instruction> fetchDecodeWithModRegRm(const instr::Opcode& opcode,
                                                                    const instr::ModRegRm& modRegRm) const;

        /// The instruction pointer is an offset within the code segment that points to the next instruction in memory.
        OffsetAddr instructionPointer = 0;

        /// CPU flag register. Declared private as not all flags should be directly modifiable by all.
        reg::Flags flags;
    };
}