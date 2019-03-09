#pragma once

#include <variant>
#include "primitives.hpp"
#include "emu/cpu/registerindexes.hpp"
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

        u8 getRmBits() const;
        
        u8 getRegBits() const;
        std::variant<GeneralIndex, IndexIndex> getRegisterIndex(DataSize size) const;

        u8 getModBits() const;
        AddressingMode getAddressingMode() const;

    private:
        u8 value;
    };
}