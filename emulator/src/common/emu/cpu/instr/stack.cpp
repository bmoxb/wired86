#include "emu/cpu/instr/stack.hpp"

#include "convert.hpp"
#include "logging.hpp"
#include "emu/cpu/intel8086.hpp"
#include "emu/cpu/reg/registers8086.hpp"

namespace emu::cpu::instr {
    PushTakingRegister::PushTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg)
    : InstructionTakingRegister("push", instrOpcode, generalReg) {}

    OffsetAddr PushTakingRegister::execute(Intel8086& cpu, Mem& memory) {
        u16 value = cpu.generalRegisters.get(registerIndex);
        cpu.pushWordToStack(value, memory);

        return cpu.getRelativeInstructionPointer() + getRawSize();
    }

    PopTakingRegister::PopTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg)
    : InstructionTakingRegister("pop", instrOpcode, generalReg) {}

    OffsetAddr PopTakingRegister::execute(Intel8086& cpu, Mem& memory) {
        u16 value = cpu.popWordFromStack(memory);
        cpu.generalRegisters.set(registerIndex, value);

        return cpu.getRelativeInstructionPointer() + getRawSize();
    }
}