#include "executor.hpp"

#include "logging.hpp"

namespace cli {
    Executor::Executor(emu::AbsAddr memorySize) : memory(memorySize) {}

    bool Executor::runCycle() {
        auto addr = cpu.nextInstructionAddress();
        logging::info("Fetching instruction from address: " + convert::toHexString(addr));
        auto instruction = cpu.fetchDecodeInstruction(addr, memory);

        if(instruction) {
            logging::success("Instruction fetched and decoded successfully!");

            logging::info("Instruction raw data: " + instruction->getRawDataString());
            logging::info("Instruction assembly: " + cpu.getInstructionAssembly(instruction));

            cpu.executeInstruction(instruction, memory);

            logging::success("Instruction executed successfully!");
            return true;
        }
        
        logging::error("Failed to decode instruction - halting...");
        return false;
    }

    void Executor::runCycles(unsigned int count) {
        bool success = true;

        for(int cycle = 0; cycle < count && success; cycle++) {
            logging::info("--- CYCLE " + std::to_string(cycle) + " ---");
            success = runCycle();
        }
    }
}