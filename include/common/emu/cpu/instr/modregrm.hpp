#pragma once

#include "primitives.hpp"
#include "emu/cpu/registers.hpp"
#include "emu/cpu/instr/opcode.hpp"

namespace emu::cpu::instr {
    enum AddressingMode {
        NO_DISPLACEMENT,
        BYTE_DISPLACEMENT,
        WORD_DISPLACEMENT,
        REGISTER_ADDRESSING_MODE
    };

    enum DisplacementType {
        BX_SI_DISPLACEMENT,
        BX_DI_DISPLACEMENT,
        BP_SI_DISPLACEMENT,
        BP_DI_DISPLACEMENT,
        SI_DISPLACEMENT,
        DI_DISPLACEMENT,
        BP_DISPLACEMENT,
        BX_DISPLACEMENT
    };

    class ModRegRm {
    public:
        ModRegRm(u8 modRegRmValue);

        /**
         * Fetch the three bits that comprise the R/M component of this MOD-REG-R/M byte.
         */
        u8 getRmBits() const;
        
        /**
         * Fetch the three bits that make up the REG component of this MOD-REG-R/M byte.
         */
        u8 getRegBits() const;

        /**
         * Fetch the pair of bits that comprise the MOD component of this MOD-REG-R/M byte.
         */
        u8 getModBits() const;

        /**
         * Return the appropriate AddressingMode enumeration value based on the MOD component bits.
         */
        AddressingMode getAddressingMode() const;

        /**
         * Get the appropriate register index based on the value of the R/M component.
         * Only relevant when using register addressing mode.
         */
        GeneralRegister getRegisterIndexFromRm(DataSize size) const;

        /**
         * Returns the appropriate register part based on the value of the R/M component.
         * Only relevant when using register addressing mode.
         */
        RegisterPart getRegisterPartFromRm(DataSize size) const;

        /**
         * Get the appropriate register index based on the value of the REG component.
         */
        GeneralRegister getRegisterIndexFromReg(DataSize size) const;

        /**
         * Returns the appropriate register part based on the value of the REG component.
         */
        RegisterPart getRegisterPartFromReg(DataSize size) const;

        /**
         * Returns the appropriate displacement type based on the value of the R/M component.
         */
        DisplacementType getDisplacementType() const;

        const u8 value;

    private:
        /**
         * Returns the appropriate register index based on 3 bits given and the data size.
         *
         * @param The 3 bits of either a REG or R/M component that specify a register.
         * @param size The data size handled by this instruction (16-bit word or 8-bit byte).
         * @return A general register index.
         */
        GeneralRegister getRegisterIndex(u8 bits, DataSize size) const;

        /**
         * Returns the appropriate register part (low byte, high byte, or full word) based on the 3 bits given and the
         * the data size. Will always return full word when given a 16-bit data size option.
         *
         * @param The 3 bits of either a REG or R/M component that specify a register.
         * @param size The data size handled by this instruction (16-bit word or 8-bit byte).
         * @return A register part.
         */
        RegisterPart getRegisterPart(u8 bits, DataSize size) const;
    };
}