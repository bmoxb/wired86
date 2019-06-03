#include "emu/cpu/intel8086.hpp"

#include "logging.hpp"
#include "emu/cpu/instr/stack.hpp"
#include "emu/cpu/instr/arithmeticlogic.hpp"

namespace emu::cpu {
    AbsAddr Intel8086::resolveAddress(OffsetAddr offset, reg::SegmentRegister segment) const {
        OffsetAddr segmentAddress = segmentRegisters.get(segment);

        return (segmentAddress << 4) + offset;
    }

    OffsetAddr Intel8086::getRelativeInstructionPointer() const {
        return instructionPointer;
    }

    AbsAddr Intel8086::getAbsoluteInstructionPointer() const {
        return resolveAddress(instructionPointer, reg::CODE_SEGMENT);
    }

    std::unique_ptr<instr::Instruction> Intel8086::fetchDecodeInstruction(AbsAddr address, const Mem& memory) const {
        MemValue opcodeValue = memory.read(address);
        instr::Opcode opcode(opcodeValue);

        std::unique_ptr<instr::Instruction> instruction;

        // First attempt to decode instruction without MOD-REG-R/M byte:
        instruction = fetchDecodeWithoutModRegRm(opcode);

        if(!instruction) {
            MemValue modRegRmValue = memory.read(address + 1); // MOD-REG-R/M byte immediately follows opcode.
            instr::ModRegRm modRegRm(modRegRmValue);

            // Attempt to decode instruction with a MOD-REG-R/M byte:
            instruction = fetchDecodeWithModRegRm(opcode, modRegRm);
        }

        if(!instruction)
            logging::warning("Encountered instruction with nonexistent or currently unimplemented opcode: " +
                             opcode.toString());

        return instruction;
    }

    std::unique_ptr<instr::Instruction> Intel8086::fetchDecodeWithoutModRegRm(const instr::Opcode& opcode) const {
        switch(opcode.value) {
        case 0x50: // PUSH AX
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::AX_REGISTER);

        case 0x51: // PUSH CX
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::CX_REGISTER);

        case 0x52: // PUSH DX
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::DX_REGISTER);

        case 0x53: // PUSH BX
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::BX_REGISTER);

        case 0x54: // PUSH SP
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::STACK_POINTER);

        case 0x55: // PUSH BP
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::BASE_POINTER);

        case 0x56: // PUSH SI
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::SOURCE_INDEX);

        case 0x57: // PUSH DI
            return std::make_unique<instr::PushTakingRegister>(opcode, reg::DESTINATION_INDEX);

        case 0x58: // POP AX
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::AX_REGISTER);

        case 0x59: // POP CX
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::CX_REGISTER);

        case 0x5A: // POP DX
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::DX_REGISTER);

        case 0x5B: // POP BX
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::BX_REGISTER);

        case 0x5C: // POP SP
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::STACK_POINTER);

        case 0x5D: // POP BP
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::BASE_POINTER);

        case 0x5E: // POP SI
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::SOURCE_INDEX);

        case 0x5F: // POP DI
            return std::make_unique<instr::PopTakingRegister>(opcode, reg::DESTINATION_INDEX);
        }

        return {};
    }

    std::unique_ptr<instr::Instruction> Intel8086::fetchDecodeWithModRegRm(const instr::Opcode& opcode,
                                                                           const instr::ModRegRm& modRegRm) const {
        switch(opcode.getUniqueValue()) {
        case 0b000000: // ADD E, G
            return std::make_unique<instr::AddEG>(opcode, modRegRm);
        }

        return {};
    }

    void Intel8086::executeInstruction(std::unique_ptr<instr::Instruction>& instruction, Mem& memory) {
        if(instruction) {
            OffsetAddr newIp = instruction->execute(*this, memory);

            if(memory.withinBounds(newIp)) instructionPointer = newIp;
            else logging::error("Instruction returned new instruction pointer value that is out of bounds!");
        }
        else logging::error("Empty instruction pointer passed to CPU.");
    }

    void Intel8086::pushToStack(MemValue value, Mem& memory) {
        OffsetAddr stackPointer = generalRegisters.get(reg::STACK_POINTER);
        
        if(stackPointer > 0) {
            stackPointer--;

            AbsAddr address = resolveAddress(stackPointer, reg::STACK_SEGMENT);
            memory.write(address, value);

            generalRegisters.set(reg::STACK_POINTER, stackPointer);
        }
        else logging::warning("Stack pointer is zero so cannot successfully push value: " +
                              convert::toHexString(value));
    }

    MemValue Intel8086::popFromStack(const Mem& memory) {
        OffsetAddr initialStackPointer = generalRegisters.get(reg::STACK_POINTER);
        generalRegisters.set(reg::STACK_POINTER, initialStackPointer + 1);

        AbsAddr address = resolveAddress(initialStackPointer, reg::STACK_SEGMENT);
        return memory.read(address);
    }

    void Intel8086::pushWordToStack(u16 value, Mem& memory) {
        pushToStack(convert::getMostSigByte(value), memory);
        pushToStack(convert::getLeastSigByte(value), memory);
    }

    u16 Intel8086::popWordFromStack(const Mem& memory) {
        u8 low = popFromStack(memory);
        u8 high = popFromStack(memory);
        return convert::createWordFromBytes(low, high);
    }

    void Intel8086::performRelativeJump(OffsetAddr offset) {
        instructionPointer = offset;
        // TODO: Ensure the jump is within bounds.
    }
}