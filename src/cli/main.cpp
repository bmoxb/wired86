#include "emu/memory.hpp"
#include "emu/cpu/intel8086.hpp"

int main(int argc, char* argv[]) {
    emu::cpu::Intel8086 cpu;
    
    emu::Memory<u8, u32> mem(0xFF);
    mem.write(0, 0b10101010);

    cpu.fetchDecodeInstruction(0, mem);

    return 0;
}