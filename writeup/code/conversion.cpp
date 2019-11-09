#include "conversion.hpp"

namespace conversion {
    u8 getHighByte(u16 value) {
        return (value >> 8); // 8 bit right shift.
    }

    u8 getLowByte(u16 value) {
        return value & 0xFF; // Extract initial byte using bitwise AND.
    }
}
