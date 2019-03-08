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

    /**
     * Create a 16-bit word from a low and a high byte.
     *
     * @param low The least significant byte.
     * @param high The most significant byte.
     * @return 16-bit word constructed from the low/high bytes.
     */
    u16 createWordFromBytes(u8 low, u8 high);

    /**
     * Fetch a specific bit of a numerical value (expressed as a boolean).
     *
     * @tparam T Numerical type to fetch bit from.
     * @param value Value to fetch bit of.
     * @param index Specify which bit to fetch. Indexing begins from 0 with the least significant bit up to the most
     *              significant.
     * @return The fetched bit expressed as a boolean value.
     */
    template <typename T>
    bool getBitFrom(T value, unsigned int index) {
        return (value >> index) & 1;
    }

    /**
     * Fetch multiple bits from a numerical value.
     *
     * @tparam T Numerical type to fetch bits from.
     * @param value Value to fetch bits from.
     * @param index Specify the beginning of the sequence of bits to fetch (indexing beginning from 0).
     * @param count The number of bits to fetch starting from the specified index.
     * @return The fetched sequence of bits.
     */
    template <typename T>
    T getBitsFrom(T value, unsigned int index, unsigned int count) {
        T mask = (1 << count) - 1; // (2 ^ count) - 1
        return (value >> index) & mask;
    }
}