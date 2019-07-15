#pragma once

#include <vector>
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/types.hpp"
#include "assembly.hpp"

namespace emu::cpu::instr {
    /**
     * Represents an additional argument to an instruction (either an immediate value or displacement value - not a
     * MOD-REG-R/M component or the opcode).
     */
    class DataArgument {
    public:
        /**
         * Create a data argument containing the specified raw data (little endian format).
         */
        DataArgument(std::vector<u8> raw);

        /**
         * Convert this instruction argument into assembly. This method is pure virtual but overriden in the Immediate
         * and Displacement classes.
         */
        virtual std::string toAssembly(DataSize size, const ModRegRm& modRegRm, const reg::GeneralRegisters& registers,
                                       const assembly::Style& style) const = 0;

        const std::vector<u8>& getRawData() const;

        /**
         * Will return the value of this immediate instruction component as either 8-bits or 16-bits (note the return
         * value will be cast to u16 regardless) based on the data size specified.
         */
        u16 getValueUsingDataSize(DataSize size) const;

    protected:
        u8 getByteValue() const;
        u16 getWordValue() const;

    private:
        std::vector<u8> rawData;
    };



    class Immediate : public DataArgument {
    public:
        using DataArgument::DataArgument;

        std::string toAssembly(DataSize size, const ModRegRm&, const reg::GeneralRegisters&,
                               const assembly::Style& style) const override final;
    };



    class Displacement : public DataArgument {
    public:
        using DataArgument::DataArgument;

        std::string toAssembly(DataSize, const ModRegRm& modRegRm, const reg::GeneralRegisters& registers,
                               const assembly::Style& style) const override final;

        /**
         *
         */
        u16 getValueUsingAddressingMode(AddressingMode mode) const;

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