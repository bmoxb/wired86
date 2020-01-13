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
