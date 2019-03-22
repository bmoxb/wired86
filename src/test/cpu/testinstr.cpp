#include "catch.hpp"
#include "primitives.hpp"
#include "emu/cpu/intel8086.hpp"

TEST_CASE("Test CPU instruction execution.", "[emu][cpu][instructions]") {
    using namespace emu;

    Mem memory(0xFF);

    cpu::Intel8086 cpu;

    cpu.generalRegisters.set(cpu::reg::STACK_POINTER, 0xAA);
    cpu.segmentRegisters.set(cpu::reg::STACK_SEGMENT, 0);

    SECTION("Test CPU stack handling.") {
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
        auto pushPopFunc = [&memory, &cpu](u8 pushOpcode, u8 popOpcode, cpu::reg::GeneralRegister reg, u16 value) {
            // Write value to register:
            cpu.generalRegisters.set(reg, value);

            // Push value in register to stack:
            memory.write(0, pushOpcode);
            auto push = cpu.fetchDecodeInstruction(0, memory);
            cpu.executeInstruction(push, memory);

            cpu.generalRegisters.set(reg, 0); // Reset register value.

            // Pop value off stack back into register:
            memory.write(1, popOpcode);
            auto pop = cpu.fetchDecodeInstruction(1, memory);
            cpu.executeInstruction(pop, memory);

            return cpu.generalRegisters.get(reg); // Return popped value in register.
        };

        REQUIRE(pushPopFunc(0x50, 0x58, cpu::reg::AX_REGISTER, 0xA) == 0xA);
        REQUIRE(pushPopFunc(0x51, 0x59, cpu::reg::CX_REGISTER, 0xC) == 0xC);
        REQUIRE(pushPopFunc(0x52, 0x5A, cpu::reg::DX_REGISTER, 0xD) == 0xD);
        REQUIRE(pushPopFunc(0x53, 0x5B, cpu::reg::BX_REGISTER, 0xB) == 0xB);
        REQUIRE(pushPopFunc(0x55, 0x5D, cpu::reg::BASE_POINTER, 0xDEAD) == 0xDEAD);
        REQUIRE(pushPopFunc(0x56, 0x5E, cpu::reg::SOURCE_INDEX, 0xBED) == 0xBED);
        REQUIRE(pushPopFunc(0x57, 0x5F, cpu::reg::DESTINATION_INDEX, 0xFEED) == 0xFEED);

        // TODO: Test push/pop stack pointer.
    }
}