#pragma once

#include "primitives.hpp"

namespace convert {
    /**
     * Returns the most significant byte of the given value.
     *
     * @param value Value to fetch high byte of.
     * @return Most significant byte of value.
     */
    u8 getHighByte(u16 value);

    /**
     * Returns the least significant byte of the given value.
     *
     * @param value Value to fetch the low byte of.
     * @return Least significant byte of value.
     */
    u8 getLowByte(u16 value);
}