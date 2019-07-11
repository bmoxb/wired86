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

        /**
         * Will return the value of this immediate instruction component as either 8-bits or 16-bits (note the return
         * value will be cast to u16 regardless) based on the data size specified.
         */
        u16 getValue(DataSize size) const;
        u8 getByteValue() const;
        u16 getWordValue() const;

    private:
        std::vector<u8> rawData; // This was original public but const however that would make this class non-copyable.
    };

    class Displacement : public Immediate {
    public:
        using Immediate::Immediate;

        std::string toAssembly(DataSize size, const ModRegRm& modRegRm, const reg::GeneralRegisters& registers,
                               std::string begin = "[", std::string end = "]", std::string separator = " + ") const;

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