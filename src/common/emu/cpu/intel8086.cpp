#include "emu/cpu/intel8086.hpp"

#include "logging.hpp"
#include "convert.hpp"
#include "emu/cpu/instr/add.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, SegmentRegister segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        return (segmentAddress << 4) + offset;
    }

    AbsAddr Intel8086::nextInstructionAddress() const {
        return resolveAddress(instructionPointer, CODE_SEGMENT);
    }

    InstructionPtr Intel8086::fetchDecodeInstruction(AbsAddr address, const Mem& memory) const {
        MemValue opcodeValue = memory.read(address);
        instr::Opcode opcode(opcodeValue);

        logging::info("From address " + convert::toHexString(address) +
                      " fetched instruction opcode: " + opcode.toString());

        switch(opcode.getUniqueValue()) { // 6 most significant bits (ignore w and d bits for now).
        /*
         * ADD instruction (with MOD-REG-R/M byte)
         * 000000xx
         */
        case 0b000000:
            MemValue modRegRmValue = memory.read(address + 1);
            instr::ModRegRm modRegRm(modRegRmValue);

            return std::make_unique<instr::Add>(opcode/*, modRegRm*/);
        }

        return InstructionPtr(); // Empty unique pointer returned otherwise.
    }
    /*
    void Intel8086::executeInstruction(InstructionUniquePtr& instruction,
                                       const Memory<MemValue, AbsAddr>& memory) {}*/
}