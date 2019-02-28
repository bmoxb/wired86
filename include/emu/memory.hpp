#pragma once

#include <memory>
#include <vector>
#include <exception>

namespace emu {
    template <typename Value, typename Address>
    class Memory {
    public:
        /**
         * Exception thrown when a call to read or write is supplied with an address that is out of bounds.
         */
        class OutOfBounds : public std::exception {
        public:
            OutOfBounds(Address addr) : address(addr) {}

            const Address address;
        }

    public:
        Memory(Address memorySize) : size(memorySize), mem(new Value[size]) { fill(); }

        /**
         * Check if the address passed is within bounds of the memory allocated.
         *
         * @param address The address check.
         * @return Wether the address is within bounds or not.
         */
        bool withinBounds(Address address) const {
            return address < size && addr >= 0;
        }

        /**
         * Fill all memory with the specified value (defaults to 0).
         *
         * @param value Value to fill memory with.
         */
        void fill(Value value = 0) {
            for(Address addr = 0; addr < size; addr++)
                write(addr, value);
        }

        /**
         * Read the value held in memory at the given address.
         *
         * @param address The address of the value to read.
         * @return The value read.
         */
        Value read(Address address) const {
            assertWithinBounds(address);
            return mem[address];
        }

        /**
         * Read multiple values from memory.
         *
         * @param startAddress The starting address to begin reading from.
         * @param amount Number of values to read.
         * @return Vector of values read.
         */
        std::vector<Value> read(Addr startAddress, Addr amount) const {
            std::vector<Value> values;

            Address address;
            Value value;

            for(Addr offset = 0; offset < amount; offset++) {
                address = startAddress + offset;
                value = read(address);

                values.push_back(value);
            }

            return values;
        }

        /**
         * Write a value to memory at the given address.
         *
         * @param address The address to be written to.
         * @param value The value to write.
         */
        void write(Address address, Value value) {
            assertWithinBounds(address);
            mem[address] = value;
        }

        /**
         * Write multiple values to memory beginning from the specified address.
         *
         * @param address The startAddress from which to begin writing.
         * @param values Vector of values to write.
         */
        void write(Address startAddress, std::vector<Value> values) {
            Address address;
            Value value;

            for(Addr offset = 0; offset < values.size(); offset++) {
                address = startAddress + offset;
                value = values[offset];

                write(address, value);
            }
        }

        // TODO: Add methods for reading from/writing to files.

        const Address size;

    protected:
        /**
         * Checks if the given address is within bounds, throwing an OutOfBounds exception if that is not the case.
         *
         * @param address The address to check.
         */
        void assertWithinBounds(Address address) const {
            if(!withinBounds(address)) throw OutOfBounds(address);
        }

    private:
        std::unique_ptr<Value[]> mem;
    };
}