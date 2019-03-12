#pragma once

#include <variant>
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
         * Return the appropriate register index based the value of the REG bits and specified data size.
         *
         * @param size The data size handled by this instruction (16-bit word or 8-bit byte).
         * @return A variant (type-safe union) holding either a general-purpose register index or an indexing register
         *         index.
         */
        std::variant<GeneralRegister, IndexRegister> getRegisterIndex(DataSize size) const;

        /**
         * Fetch the pair of bits that comprise the MOD component of this MOD-REG-R/M byte.
         */
        u8 getModBits() const;

        /**
         * Return the appropriate AddressingMode enumeration value based on the MOD component bits.
         */
        AddressingMode getAddressingMode() const;

    private:
        u8 value;
    };
}