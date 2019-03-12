#include "convert.hpp"

namespace convert {
    u8 getMostSigByte(u16 value) {
        return (value >> 8) & 0xFF;
    }

    u8 getLeastSigByte(u16 value) {
        return value & 0xFF;
    }

    u16 createWordFromBytes(u8 low, u8 high) {
        return (high << 8) + low;
    }

    std::string bitAsStr(bool bit) {
        return bit ? "1" : "0";
    }
}