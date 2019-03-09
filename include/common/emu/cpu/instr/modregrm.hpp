#pragma once

#include "primitives.hpp"

namespace emu::cpu::instr {
    enum Reg {};
    enum AddressMode {};
    enum RmDisplacement {};

    class ModRegRm {
    public:
        ModRegRm(u8 modRegRmValue);

        u8 getRmBits() const;
        u8 getRegBits() const;
        u8 getModBits() const;

    private:
        u8 value;
    };
}