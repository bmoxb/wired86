#include "emu/cpu/instr/stack.hpp"

#include "convert.hpp"
#include "logging.hpp"
#include "emu/cpu/intel8086.hpp"

namespace emu::cpu::instr {
    PushTakingRegister::PushTakingRegister(Opcode instrOpcode, GeneralRegister reg)
    : InstructionTakingRegister("push", instrOpcode, reg) {}

    OffsetAddr PushTakingRegister::execute(Intel8086& cpu, OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters,
                                           SegmentRegs&, Flags&) {
        u16 value = generalRegisters.get(registerIndex);
        cpu.pushWordToStack(value, memory);

        return ip + getRawSize();
    }

    PopTakingRegister::PopTakingRegister(Opcode instrOpcode, GeneralRegister reg)
    : InstructionTakingRegister("pop", instrOpcode, reg) {}

    OffsetAddr PopTakingRegister::execute(Intel8086& cpu, OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters,
                                          SegmentRegs&, Flags&) {
        u16 value = cpu.popWordFromStack(memory);
        generalRegisters.set(registerIndex, value);

        return ip + getRawSize();
    }
}