#pragma once

#include "emu/types.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/intel8086.hpp"
#include "assembly.hpp"

namespace cli {
    class Executor {
    public:
        Executor(emu::AbsAddr memorySize, std::string path);

        /**
         * Complete a single CPU cycle.
         *
         * @return Whether the cycle completed successfully or not.
         */
        bool runCycle();

        /**
         * Complete multiple CPU cycles.
         *
         * @param count The number of cycles to execute.
         * @return Returns the number of CPU cycles executed successfully.
         */
        unsigned int runCycles(unsigned int count);

    protected:
        emu::Memory<emu::MemValue, emu::AbsAddr> memory;
        emu::cpu::Intel8086 cpu;
        assembly::Style asmStyle;
    };
}