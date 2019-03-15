#pragma once

#include <vector>
#include "primitives.hpp"

namespace emu::cpu::instr {
    class Immediate {
    public:
        /**
         * Create immediate instruction argument.
         *
         * @param raw The raw immediate value express as either 1 or 2 bytes in little endian ordering.
         */
        Immediate(std::vector<u8> raw);

        const std::vector<u8> rawData;
    };

    class Displacement : public Immediate {};
}