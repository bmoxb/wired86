#include "emu/cpu/instr/immediate.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    Immediate::Immediate(std::vector<u8> raw) : rawData(raw) {} // TODO: Check length of raw.

    u8 Immediate::getByteValue() const {
        return rawData[0];
    }

    u16 Immediate::getWordValue() const {
        return convert::createWordFromBytes(rawData[0], rawData[1]);
    }
}