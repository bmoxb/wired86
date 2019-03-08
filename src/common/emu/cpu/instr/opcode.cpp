#include "emu/cpu/instr/opcode.hpp"

namespace emu::cpu::instr {
    Opcode::Opcode(u8 opcodeValue) : value(opcodeValue) {}

    bool Opcode::getWordBit() const {
        return value & 1; // Least significant bit.
    }

    DataSize Opcode::getDataSize() const {
        return getWordBit() ? WORD_DATA_SIZE // w=1
                            : BYTE_DATA_SIZE; // w=0
    }

    bool Opcode::getDirectionBit() const {
        return (value >> 1) & 1; // Second-to-least significant bit.
    }

    RegDirection Opcode::getDirection() const {
        return getDirectionBit() ? REG_IS_DESTINATION // d=1
                                 : REG_IS_SOURCE; // d=0
    }
}