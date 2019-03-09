#include "emu/cpu/intel8086.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, SegmentIndex segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        return (segmentRegisters << 4) + offset;
    }

    AbsAddr Intel8086::nextInstructionAddress() const {
        return resolveAddress(instructionPointer, SegmentIndex::code);
    }

    std::unique_ptr<Instruction> Intel8086::fetchDecodeInstruction(AbsAddr address,
                                                                   const Memory<MemValue, AbsAddr>& memory) const {}
    
    void Intel8086::executeInstruction(std::unique_ptr<Instruction>& instruction,
                                       const Memory<MemValue, AbsAddr>& memory) {}
}