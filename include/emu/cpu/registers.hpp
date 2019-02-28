#pragma once

#include <map>
#include "primitives.hpp"

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
        Value get(Index index) { return regs[index]; }
        void set(Index index, Value value) { regs[index] = value; }

    private:
        std::map<Indexes, Value> regs;
    };

    template <typename Index>
    class RegistersLowHigh : public Registers<Index, u16> {
    public:
        Value getLow(Index index) {}
        Value getHigh(Index index) {}
        Value get(Index index, RegisterPart part) {}

        void setLow(Index index, u16 value) {}
        void setHigh(Index index, u16 value) {}
        void set(Index index, RegisterPart part, u16 value) {}
    }
}