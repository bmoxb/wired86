#pragma once

#include <vector>
#include "primitives.hpp"

namespace emu::cpu::instr {
    class Immediate {
    public:
        Immediate(std::vector<u8> raw);

        const std::vector<u8> rawData;
    };
}