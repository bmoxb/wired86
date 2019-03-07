#include "convert.hpp"

namespace convert {
    u8 getHighByte(u16 value) {
        return (value >> 8) & 0xFF;
    }

    u8 getLowByte(u16 value) {
        return value & 0xFF;
    }
}