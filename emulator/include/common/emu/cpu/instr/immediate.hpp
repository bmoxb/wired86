#pragma once

#include <vector>
#include "emu/types.hpp"
#include "emu/cpu/instr/modregrm.hpp"

namespace emu::cpu::instr {
    class Immediate {
    public:
        /**
         * Create immediate instruction argument.
         *
         * @param raw The raw immediate value express as either 1 or 2 bytes in little endian ordering.
         */
        Immediate(std::vector<u8> raw);

        const std::vector<u8>& getRawData() const;
        u8 getByteValue() const;
        u16 getWordValue() const;

    private:
        std::vector<u8> rawData; // This was original public but const however that would make this class non-copyable.
    };

    class Displacement : public Immediate {
    public:
        using Immediate::Immediate;

        /**
         * Returns an absolute memory address based on the displacement value and displacement type.
         *
         * @param mode The addressing mode specified by the MOD-REG-R/M byte (note that register addressing mode will
         *        be treated the same as no displacement).
         * @param type The displacement type as specified by the MOD-REG-R/M byte.
         * @param registers The general-purpose and indexing CPU registers.
         * @return The resolved absolute memory address.
         */
        AbsAddr resolve(AddressingMode mode, DisplacementType type, reg::GeneralRegisters& registers) const;
    };
}