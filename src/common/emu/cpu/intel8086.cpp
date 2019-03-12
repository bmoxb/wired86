#include "emu/cpu/intel8086.hpp"

#include "logging.hpp"
#include "convert.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, SegmentRegister segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        return (segmentAddress << 4) + offset;
    }

    AbsAddr Intel8086::nextInstructionAddress() const {
        return resolveAddress(instructionPointer, CODE_SEGMENT);
    }

    InstructionUniquePtr Intel8086::fetchDecodeInstruction(AbsAddr address,
                                                           const Memory<MemValue, AbsAddr>& memory) const {
        MemValue opcodeValue = memory.read(address);
        instr::Opcode opcode(opcodeValue);

        logging::info("From address " + convert::toHexString(address) +
                      " fetched instruction opcode: " + convert::toBinaryString<MemValue, 8>(opcodeValue));

        return InstructionUniquePtr();
    }
    /*
    void Intel8086::executeInstruction(InstructionUniquePtr& instruction,
                                       const Memory<MemValue, AbsAddr>& memory) {}*/
}