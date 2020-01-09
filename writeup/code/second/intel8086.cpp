#include "emu/cpu/intel8086.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, SegmentIndex segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        // Calculate 20-bit absolute address by applying 4-bit left shift to segment address and then adding the offset:
        return (segmentRegisters << 4) + offset;
    }

    AbsAddr Intel8086::nextInstructionAddress() const {
        // The next instruction for execution will be at the instruction pointer segmented within the code segment:
        return resolveAddress(instructionPointer, SegmentIndex::CODE);
    }

    std::unique_ptr<Instruction> Intel8086::fetchDecodeInstruction(AbsAddr address, const Memory<MemValue, AbsAddr>& memory) const {
        // ...
    }
    
    void Intel8086::executeInstruction(std::unique_ptr<Instruction>& instruction, const Memory<MemValue, AbsAddr>& memory) {
        // ...
    }
}
