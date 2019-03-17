#include "emu/cpu/intel8086.hpp"

#include "logging.hpp"
#include "emu/cpu/instr/stack.hpp"

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

        switch(opcodeValue) {
        // PUSH AX
        case 0x50: return std::make_unique<instr::PushTakingRegister>(opcode, AX_REGISTER);

        // PUSH CX
        case 0x51: return std::make_unique<instr::PushTakingRegister>(opcode, CX_REGISTER);

        // PUSH DX
        case 0x52: return std::make_unique<instr::PushTakingRegister>(opcode, DX_REGISTER);

        // PUSH BX
        case 0x53: return std::make_unique<instr::PushTakingRegister>(opcode, BX_REGISTER);

        // PUSH SP
        case 0x54: return std::make_unique<instr::PushTakingRegister>(opcode, STACK_POINTER);

        // PUSH BP
        case 0x55: return std::make_unique<instr::PushTakingRegister>(opcode, BASE_POINTER);

        // PUSH SI
        case 0x56: return std::make_unique<instr::PushTakingRegister>(opcode, SOURCE_INDEX);

        // PUSH DI
        case 0x57: return std::make_unique<instr::PushTakingRegister>(opcode, DESTINATION_INDEX);

        // POP AX
        case 0x58: return std::make_unique<instr::PopTakingRegister>(opcode, AX_REGISTER);

        // POP CX
        case 0x59: return std::make_unique<instr::PopTakingRegister>(opcode, CX_REGISTER);

        // POP DX
        case 0x5A: return std::make_unique<instr::PopTakingRegister>(opcode, DX_REGISTER);

        // POP BX
        case 0x5B: return std::make_unique<instr::PopTakingRegister>(opcode, BX_REGISTER);

        // POP SP
        case 0x5C: return std::make_unique<instr::PopTakingRegister>(opcode, STACK_POINTER);

        // POP BP
        case 0x5D: return std::make_unique<instr::PopTakingRegister>(opcode, BASE_POINTER);

        // POP SI
        case 0x5E: return std::make_unique<instr::PopTakingRegister>(opcode, SOURCE_INDEX);

        // POP DI
        case 0x5F: return std::make_unique<instr::PopTakingRegister>(opcode, DESTINATION_INDEX);
        }

        logging::warning("Failed to decode instruction with opcode: " + opcode.toString());
        return {};
    }

    void Intel8086::executeInstruction(std::unique_ptr<instr::Instruction>& instruction, Mem& memory) {
        if(instruction) {
            OffsetAddr newIp = instruction->execute(*this, instructionPointer, memory,
                                                    generalRegisters, segmentRegisters, flags);

            if(memory.withinBounds(newIp)) instructionPointer = newIp;
            else logging::error("Instruction returned new instruction pointer value that is out of bounds!");
        }
        else logging::error("Empty instruction pointer passed to CPU.");
    }

    void Intel8086::pushToStack(MemValue value, Mem& memory) {
        OffsetAddr stackPointer = generalRegisters.get(STACK_POINTER) - 1;
        
        if(stackPointer > 0) {
            AbsAddr address = resolveAddress(stackPointer, STACK_SEGMENT);
            memory.write(address, value);

            generalRegisters.set(STACK_POINTER, stackPointer);
        }
        else logging::warning("Attempted to push to stack when stack pointer is zero!");
    }

    MemValue Intel8086::popFromStack(Mem& memory) {
        OffsetAddr initialStackPointer = generalRegisters.get(STACK_POINTER);
        generalRegisters.set(STACK_POINTER, initialStackPointer + 1);

        AbsAddr address = resolveAddress(initialStackPointer, STACK_SEGMENT);
        return memory.read(address);
    }

    void Intel8086::pushWordToStack(u16 value, Mem& memory) {
        pushToStack(convert::getMostSigByte(value), memory);
        pushToStack(convert::getLeastSigByte(value), memory);
    }

    u16 Intel8086::popWordFromStack(Mem& memory) {
        u8 low = popFromStack(memory);
        u8 high = popFromStack(memory);
        return convert::createWordFromBytes(low, high);
    }
}