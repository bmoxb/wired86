#include "emu/cpu/intel8086.hpp"

#include "logging.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, SegmentRegister segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        return (segmentAddress << 4) + offset;
    }

    AbsAddr Intel8086::nextInstructionAddress() const {
        return resolveAddress(instructionPointer, CODE_SEGMENT);
    }

    std::unique_ptr<instr::Instruction> Intel8086::fetchDecodeInstruction(AbsAddr address, const Mem& memory) const {
        MemValue opcodeValue = memory.read(address);
        instr::Opcode opcode(opcodeValue);

        switch(opcode.getUniqueValue()) { // The 6 most significant bits of the opcode (ignore w and d bits for now).
        /*
         * ADD instruction (with MOD-REG-R/M byte)
         * 000000xx
         */
        case 0b000000:
            MemValue modRegRmValue = memory.read(address + 1);
            instr::ModRegRm modRegRm(modRegRmValue);

            //return std::make_unique<instr::Add>(opcode/*, modRegRm*/);
        }

        logging::warning("Failed to decode instruction with opcode: " + opcode.toString());
        return {};
    }

    void Intel8086::executeInstruction(std::unique_ptr<instr::Instruction>& instruction,
                                       Memory<MemValue, AbsAddr>& memory) {
        if(instruction) {
            OffsetAddr newIp = instruction->execute(instructionPointer, memory, generalRegisters, segmentRegisters,
                                                    flags);

            if(memory.withinBounds(newIp)) instructionPointer = newIp;
            else logging::error("Instruction returned new instruction pointer value that is out of bounds!");
        }
        else logging::error("Empty instruction pointer passed to CPU.");
    }
}