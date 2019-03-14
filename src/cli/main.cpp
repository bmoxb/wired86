#include "logging.hpp"
#include "convert.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/intel8086.hpp"

int main(int argc, char* argv[]) {
    emu::cpu::Intel8086 cpu;
    
    emu::Memory<u8, u32> mem(0xFF);
    mem.write(0, 0b10101010);

    for(int i = 0; i < 5; i++) {
        auto addr = cpu.nextInstructionAddress();
        logging::info("Next instruction address: " + convert::toHexString(addr));

        auto instruction = cpu.fetchDecodeInstruction(addr, mem);
        
        logging::success("Instruction fetched and decoded successfully!");
        logging::info("Instruction raw data: " + instruction->getRawDataString());
        logging::info("Instruction assembly: " + instruction->toAssembly());

        cpu.executeInstruction(instruction, mem);
    }

    return 0;
}