#include "executor.hpp"

#include "logging.hpp"

namespace cli {
    Executor::Executor(emu::AbsAddr memorySize, std::string path) : memory(memorySize) {
        memory.loadFromFile(path);
    }

    bool Executor::runCycle() {
        auto addr = cpu.getAbsoluteInstructionPointer();
        logging::info("Fetching instruction from address: " + convert::toHexString(addr));
        auto instruction = cpu.fetchDecodeInstruction(addr, memory);

        if(instruction) {
            logging::success("Instruction fetched and decoded successfully: " + instruction->toAssembly(cpu));
            logging::info("Instruction raw data: " + instruction->getRawDataString());

            bool success = cpu.executeInstruction(instruction, memory);

            if(success) logging::success("Instruction executed successfully!");
            else logging::error("Instruction failed to execute successfully.");

            return success;
        }

        logging::error("Failed to decode instruction - halting...");
        cpu.halted = true;

        return false;
    }

    unsigned int Executor::runCycles(unsigned int count) {
        unsigned int cycle;
        bool success = true;

        for(cycle = 1; cycle <= count && success; cycle++) {
            logging::info("--- CYCLE " + std::to_string(cycle) + " ---");
            success = runCycle();

            if(cpu.halted) {
                logging::warning("Detected that CPU is now in halted state. Remaining cycles will not be executed.");
                logging::info("--- TOTAL " + std::to_string(cycle) + " OF " + std::to_string(count) +
                              " CYCLES COMPLETED ---");
                
                return cycle;
            }
        }

        logging::info("--- ALL " + std::to_string(count) + " CYCLES COMPLETED ---");
        return count;
    }
}