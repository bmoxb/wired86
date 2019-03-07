#pragma once

#include <map>
#include "primitives.hpp"
#include "convert.hpp"
#include "emu/cpu/registerindexes.hpp"

namespace emu::cpu {
    /**
     * Generic class template for a collection of registers.
     *
     * @tparam Index Type of indexes for specifying the desired register (should be a RegisterIndex index).
     * @tparam Value Type of value stored in each register (usually numerical).
     */
    template <typename Index, typename Value>
    class Registers {
    public:
        /// Register value getter.
        Value get(Index index) { return regs[index]; }

        /// Register value setter.
        void set(Index index, Value value) { regs[index] = value; }

    private:
        std::map<Index, Value> regs;
    };

    template <typename Index>
    class RegistersLowHigh : public Registers<Index, u16> {
    public:
        /// Get least significant byte of 16-bit register.
        u8 getLow(Index index) {
            u16 value = get(index);
            return convert::getLowByte(value);
        }

        /// Get most significant byte of 16-bit register.
        u8 getHigh(Index index) {
            u16 value = get(index);
            return convert::getHighByte(value);
        }

        /**
         * Fetch a specific part of a register. Note that return value will always be 16-bit wide even if only a single
         * byte of a register is accessed.
         */
        u16 get(Index index, RegisterPart part) {
            switch(part) {
            case LOW_BYTE: return getLow(index);
            case HIGH_BYTE: return getHigh(index);
            default: return get(index);
            }
        }

        /// Set least significant byte of 16-bit register (most significant byte unaffected).
        void setLow(Index index, u8 low) {
            u16 high = getHigh(index);
            u16 value = convert::createWordFromBytes(low, high);
            set(value);
        }

        /// Set most significant byte of 16-bit register (least significant byte unaffected).
        void setHigh(Index index, u8 high) {
            u16 low = getLow(index);
            u16 value = convert::createWordFromBytes(low, high);
            set(value);
        }

        /**
         * Set a specific part of a register. Note that the value argument is 16-bit wide but will be cast to 8-bits
         * when only a setting a high or low byte of a register and not the entire 16-bit value.
         */
        void set(Index index, RegisterPart part, u16 value) {
            u8 byte = static_cast<u8>(value);

            switch(part) {
            case LOW_BYTE: setLow(index, byte); break;
            case HIGH_BYTE: setHigh(index, byte); break;
            default: set(index, value);
            }
        }
    };
}