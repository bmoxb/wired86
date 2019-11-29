#include "common/conversion.hpp"

namespace conversion {
    u8 getHighByte(u16 value) {
        return value >> 8;
    }

    u8 getLowByte(u16 value) {
        return value & 0xFF;
    }
}
