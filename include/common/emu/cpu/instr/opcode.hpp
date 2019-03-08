#pragma once

#include "primitives.hpp"

namespace emu::cpu::instr {
    enum DataSize { WORD_DATA_SIZE, BYTE_DATA_SIZE };
    enum RegDirection { REG_IS_SOURCE, REG_IS_DESTINATION };

    class Opcode {
    public:
        Opcode(u8 opcodeValue);

        /**
         * Indicates whether the data size of this opcode is a word (when the w-bit is 1/true) or a byte (when the w-bit
         * is 0/false). This bit is the least significant bit of the opcode.
         */
        bool getWordBit() const;
        DataSize getDataSize() const;

        /**
         * Indicates whether the REG component of a MOD-REG-R/M byte is the source or destination for data handled by
         * the instruction. This bit is the second-to-least significant bit of the opcode.
         */
        bool getDirectionBit() const;
        RegDirection getDirection() const;

        const u8 value;
    };
}