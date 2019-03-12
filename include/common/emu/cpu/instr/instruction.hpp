#pragma once

#include <string>
#include <vector>
#include <memory>
#include "primitives.hpp"
#include "emu/cpu/instr/opcode.hpp"
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/cpu/instr/immediate.hpp"

namespace emu::cpu::instr {
    class Instruction {
    public:
        Instruction(Opcode instructionOpcode);

        //virtual void execute();

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         */
        std::string toAssembly() const;

        /**
         * Fetch the raw 8-bit values that make this instruction include the opcode value.
         */
        std::vector<u8> getRawData() const;

        const std::string name;
        const Opcode opcode;

    protected:
        std::unique_ptr<ModRegRm> modRegRmByte;
        //std::unique_ptr<Displacement> displacementValue;
        std::unique_ptr<Immediate> immediateValue;
    };
}