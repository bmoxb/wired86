#include "emu/cpu/instr/opcode.hpp"

#include "convert.hpp"

namespace emu::cpu::instr {
    Opcode::Opcode(u8 opcodeValue) : value(opcodeValue) {}

    std::string Opcode::toString() const {
        bool d = getDirectionBit(), w = getWordBit();

        return convert::toHexString(value) +
               " (" + convert::toBinaryString<6>(getUniqueValue(), "") + "dw : d=" + convert::bitAsStr(d) +
               ", w=" + convert::bitAsStr(w) + ")";
    }

    u8 Opcode::getUniqueValue() const {
        return convert::getBitsFrom(value, 2, 6);
    }

    bool Opcode::getWordBit() const {
        return convert::getBitFrom(value, 0); // Least significant bit.
    }

    DataSize Opcode::getDataSize() const {
        return getWordBit() ? WORD_DATA_SIZE  // w=1
                            : BYTE_DATA_SIZE; // w=0
    }

    AbsAddr Opcode::getImmediateReadLength() const {
        return getWordBit() ? 2  // Word data size (i.e. read 2 bytes).
                            : 1; // Byte data size (i.e. read 1 byte).
    }

    bool Opcode::getDirectionBit() const {
        return convert::getBitFrom(value, 1); // Second-to-least significant bit.
    }

    RegDirection Opcode::getDirection() const {
        return getDirectionBit() ? REG_IS_DESTINATION // d=1
                                 : REG_IS_SOURCE;     // d=0
    }
}
