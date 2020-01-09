TEST_CASE("Test CPU registers.", "[emu][cpu][registers]") {
    enum Index { REG };
    emu::cpu::Registers<Index, u32> regs;

    SECTION("Ensure registers are initialised to 0.") {
        REQUIRE(regs.get(REG) == 0);
    }

    SECTION("Test setting/getting register values.") {
        regs.set(REG, 0xBED);
        REQUIRE(regs.get(REG) == 0xBED);
    }

    emu::cpu::RegistersLowHigh<Index> regsLowHigh;

    SECTION("Test individual access of high/low bytes of registers.") {
        regsLowHigh.setLow(REG, 0xA);
        regsLowHigh.setHigh(REG, 0xB);

        REQUIRE(regsLowHigh.getLow(REG) == 0xA);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::LOW_BYTE) == 0xA);

        REQUIRE(regsLowHigh.getHigh(REG) == 0xB);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::HIGH_BYTE) == 0xB);

        REQUIRE(regsLowHigh.get(REG) == 0x0B0A);
        REQUIRE(regsLowHigh.get(REG, emu::cpu::FULL_WORD) == 0x0B0A);
    }
}
