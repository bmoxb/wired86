#include "logging.hpp"
#include "convert.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/intel8086.hpp"

int main(int argc, char* argv[]) {
    emu::cpu::Intel8086 cpu;
    
    emu::Memory<u8, u32> mem(0xFF);
    mem.write(0, 0x55);

    for(int i = 0; i < 5; i++) {
        auto addr = cpu.nextInstructionAddress();
        logging::info("Next instruction address: " + convert::toHexString(addr));

        auto instruction = cpu.fetchDecodeInstruction(addr, mem);

        if(instruction) {
            logging::success("Instruction fetched and decoded successfully!");

            logging::info("Instruction raw data: " + instruction->getRawDataString());
            logging::info("Instruction assembly: " + cpu.getInstructionAssembly(instruction));

            cpu.executeInstruction(instruction, mem);
        }
        else {
            logging::error("Failed to decode instruction - halting...");
            break;
        }
    }

    return 0;
}