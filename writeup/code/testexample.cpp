SECTION("Test arithmetic instructions.") {
    cpu.generalRegisters.set(cpu::reg::CX_REGISTER, 5);
    cpu.generalRegisters.set(cpu::reg::BX_REGISTER, 10);

    cpu.performRelativeJump(0); // Reset instruction pointer to 0.
    memory.write(0, { 0b00000001, 0b11011001 }); // add cx, bx
    
    auto add = cpu.fetchDecodeInstruction(0, memory);
    cpu.executeInstruction(add, memory);

    REQUIRE(add->toAssembly(cpu, assembly::Style()) == "add cx, bx"); // Test assembly generation.
    REQUIRE(cpu.generalRegisters.get(cpu::reg::CX_REGISTER) == 15); // Test execution.
}
