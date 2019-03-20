#include "emu/cpu/instr/stack.hpp"

#include "convert.hpp"
#include "logging.hpp"
#include "emu/cpu/intel8086.hpp"
#include "emu/cpu/reg/registers8086.hpp"

namespace emu::cpu::instr {
    PushTakingRegister::PushTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg)
    : InstructionTakingRegister("push", instrOpcode, generalReg) {}

    OffsetAddr PushTakingRegister::execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
                                           reg::GeneralRegisters& generalRegisters, reg::SegmentRegisters&,
                                           reg::Flags&) {
        u16 value = generalRegisters.get(registerIndex);
        cpu.pushWordToStack(value, memory);

        return ip + getRawSize();
    }

    PopTakingRegister::PopTakingRegister(Opcode instrOpcode, reg::GeneralRegister generalReg)
    : InstructionTakingRegister("pop", instrOpcode, generalReg) {}

    OffsetAddr PopTakingRegister::execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
                                          reg::GeneralRegisters& generalRegisters, reg::SegmentRegisters&,
                                          reg::Flags&) {
        u16 value = cpu.popWordFromStack(memory);
        generalRegisters.set(registerIndex, value);

        return ip + getRawSize();
    }
}