#include "catch.hpp"
#include "emu/cpu/intel8086.hpp"

TEST_CASE("Test CPU instruction execution.", "[emu][cpu][instructions]") {
    SECTION("Test CPU stack handling.") {
        emu::Mem memory(0xFF);

        emu::cpu::Intel8086 cpu;
        cpu.generalRegisters.set(emu::cpu::reg::STACK_POINTER, 20);
        cpu.segmentRegisters.set(emu::cpu::reg::STACK_SEGMENT, 0);

        cpu.pushToStack(0xAB, memory);
        cpu.pushToStack(0xCD, memory);
        cpu.pushToStack(0xEF, memory);

        REQUIRE(cpu.popFromStack(memory) == 0xEF);
        REQUIRE(cpu.popFromStack(memory) == 0xCD);
        REQUIRE(cpu.popFromStack(memory) == 0xAB);

        cpu.pushWordToStack(0xABCD, memory);
        REQUIRE(cpu.popWordFromStack(memory) == 0xABCD);
    }

    SECTION("Test stack instructions.") {
        constexpr u16 VALUE = 0xABCD;

        emu::Mem memory(0xFF);

        emu::cpu::Intel8086 cpu;
        cpu.generalRegisters.set(emu::cpu::reg::STACK_POINTER, 0xAA);
        cpu.generalRegisters.set(emu::cpu::reg::AX_REGISTER, VALUE);

        memory.write(0, 0x50); // PUSH AX
        auto pushAx = cpu.fetchDecodeInstruction(0, memory);
        cpu.executeInstruction(pushAx, memory);

        memory.write(0, 0x5B); // POP BX
        auto popBx = cpu.fetchDecodeInstruction(0, memory);
        cpu.executeInstruction(popBx, memory);

        REQUIRE(cpu.generalRegisters.get(emu::cpu::reg::BX_REGISTER) == VALUE);
    }
}