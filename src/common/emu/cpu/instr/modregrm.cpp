#include "emu/cpu/instr/modregrm.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    ModRegRm::ModRegRm(u8 modRegRmValue) : value(modRegRmValue) {}

    u8 ModRegRm::getRmBits() const {
        return convert::getBitsFrom(value, 0, 3);
    }

    u8 ModRegRm::getRegBits() const {
        return convert::getBitsFrom(value, 3, 3);
    }

    u8 ModRegRm::getModBits() const {
        return convert::getBitsFrom(value, 6, 2);
    }
}