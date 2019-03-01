#pragma once

#include <map>
#include "common/primitives.hpp"
#include "common/conversion.hpp"
#include "common/emu/cpu/registerindexes.hpp"

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
            return conversion::getLowByte(value);
        }

        /// Get most significant byte of 16-bit register.
        u8 getHigh(Index index) {
            u16 value = get(index);
            return conversion::getHighByte(value);
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

        void setLow(Index index, u8 value) {}
        void setHigh(Index index, u8 value) {}
        void set(Index index, RegisterPart part, u16 value) {}
    };
}