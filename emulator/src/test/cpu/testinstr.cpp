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
        auto testPushPopInstructions = [&memory, &cpu](u8 pushOpcode, u8 popOpcode, cpu::reg::GeneralRegister reg, u16 value) {
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

            REQUIRE(value == cpu.generalRegisters.get(reg)); // Assert that the instructions functioned correctly.
        };

        testPushPopInstructions(0x50, 0x58, cpu::reg::AX_REGISTER, 0xA);
        testPushPopInstructions(0x51, 0x59, cpu::reg::CX_REGISTER, 0xC);
        testPushPopInstructions(0x52, 0x5A, cpu::reg::DX_REGISTER, 0xD);
        testPushPopInstructions(0x53, 0x5B, cpu::reg::BX_REGISTER, 0xB);
        testPushPopInstructions(0x55, 0x5D, cpu::reg::BASE_POINTER, 0xDEAD);
        testPushPopInstructions(0x56, 0x5E, cpu::reg::SOURCE_INDEX, 0xBED);
        testPushPopInstructions(0x57, 0x5F, cpu::reg::DESTINATION_INDEX, 0xFEED);

        // TODO: Test push/pop stack pointer.
    }

    SECTION("Test arithmetic instructions.") {
        cpu.generalRegisters.set(cpu::reg::CX_REGISTER, 5);
        cpu.generalRegisters.set(cpu::reg::BX_REGISTER, 10);

        cpu.performRelativeJump(0); // Reset instruction pointer to 0.
        memory.write(0, { 0b00000001, 0b11011001 }); // add cx, bx
        
        auto add = cpu.fetchDecodeInstruction(0, memory);
        cpu.executeInstruction(add, memory);

        REQUIRE(add->toAssembly(cpu, assembly::Style()) == "add cx, bx");
        REQUIRE(cpu.generalRegisters.get(cpu::reg::CX_REGISTER) == 15);

    }
}