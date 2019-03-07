#include "convert.hpp"

namespace convert {
    u8 getHighByte(u16 value) {
        return (value >> 8) & 0xFF;
    }

    u8 getLowByte(u16 value) {
        return value & 0xFF;
    }

    u16 createWordFromBytes(u8 low, u8 high) {
        return (high << 8) + low;
    }
}