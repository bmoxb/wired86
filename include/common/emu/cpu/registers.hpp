#pragma once

#include <map>
#include "primitives.hpp"
#include "convert.hpp"

namespace emu::cpu {
    enum RegisterPart { FULL_WORD, LOW_BYTE, HIGH_BYTE };
    enum GeneralRegister { AX_REGISTER, BX_REGISTER, CX_REGISTER, DX_REGISTER };
    enum IndexRegister { SOURCE_INDEX, DESTINATION_INDEX, BASE_POINTER, STACK_POINTER };
    enum SegmentRegister { CODE_SEGMENT, DATA_SEGMENT, EXTRA_SEGMENT, STACK_SEGMENT };

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
        Value get(Index index) const {
            // Cannot have method declared `const` when using the operator[] as that method of indexing will create a
            // default constructed value if one is not found at the index (thus modifying `this`). As such, the map will
            // be searched for the index and will return a default value should a value not be found.
            auto value = regs.find(index);

            if(value != regs.end()) return value->second;
            else return Value(); // Return default-constructed value if no actual value is found in map.
        }

        /// Register value setter.
        void set(Index index, Value value) { regs[index] = value; }

    private:
        std::map<Index, Value> regs;
    };

    template <typename Index>
    class RegistersLowHigh : public Registers<Index, u16> {
    public:
        using Registers<Index, u16>::get;
        using Registers<Index, u16>::set;

        /// Get least significant byte of 16-bit register.
        u8 getLow(Index index) const {
            u16 value = get(index);
            return convert::getLeastSigByte(value);
        }

        /// Get most significant byte of 16-bit register.
        u8 getHigh(Index index) const {
            u16 value = get(index);
            return convert::getMostSigByte(value);
        }

        /**
         * Fetch a specific part of a register. Note that return value will always be 16-bit wide even if only a single
         * byte of a register is accessed.
         */
        u16 get(Index index, RegisterPart part) const {
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
            set(index, value);
        }

        /// Set most significant byte of 16-bit register (least significant byte unaffected).
        void setHigh(Index index, u8 high) {
            u16 low = getLow(index);
            u16 value = convert::createWordFromBytes(low, high);
            set(index, value);
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