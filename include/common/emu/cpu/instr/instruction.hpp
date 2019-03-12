#pragma once

#include <vector>
#include <optional>
#include "primitives.hpp"
#include "emu/cpu/instr/opcode.hpp"
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/cpu/instr/immediate.hpp"

namespace emu::cpu::instr {
    class Instruction {
    public:
        Instruction(Opcode instructionOpcode);

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         */
        std::string toAssembly() const;

        /**
         * Fetch the raw 8-bit values that make this instruction include the opcode value.
         */
        std::vector<u8> getRawData() const;

        const Opcode opcode;

    protected:
        std::optional<ModRegRm> modRegRmByte;
        //std::optional<Displacement> displacementValue;
        std::optional<Immediate> immediateValue;
    };
}