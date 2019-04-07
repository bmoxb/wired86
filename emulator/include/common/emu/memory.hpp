#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>
#include "convert.hpp"

namespace emu {
    template <typename Value, typename Address>
    class Memory {
    public:
        /**
         * Exception thrown when a call to read or write is supplied with an address that is out of bounds.
         */
        class OutOfBounds : public std::runtime_error {
        public:
            OutOfBounds(Address addr)
            : std::runtime_error("Attempted to access memory address that is out of bounds: " +
                                 convert::toHexString(addr)), address(addr) {}

            const Address address;
        };

    public:
        Memory(Address memorySize) : size(memorySize), mem(new Value[size]) { fill(); }

        /**
         * Check if the address passed is within bounds of the memory allocated.
         *
         * @param address The address check.
         * @return Wether the address is within bounds or not.
         */
        bool withinBounds(Address address) const {
            return address < size && address >= 0;
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
        std::vector<Value> read(Address startAddress, Address amount) const {
            std::vector<Value> values;

            Address address;
            Value value;

            for(Address offset = 0; offset < amount; offset++) {
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

            for(Address offset = 0; offset < values.size(); offset++) {
                address = startAddress + offset;
                value = values[offset];

                write(address, value);
            }
        }

        /**
         * Load data from a binary file into emulator memory.
         *
         * @param path The path of the file from which data should be loaded.
         * @param offset The offset in memory to which data should be loaded.
         * @return Whether the file could be opened successfully or not.
         */
        bool loadFromFile(std::string path, Address offset = 0) {
            std::ifstream file;

            file.open(path,
                      std::ios::in | // Input/read mode.
                      std::ios::binary | // Binary mode.
                      std::ios::ate); // Place cursor at end of file st that tellg will return file size.

            if(file.is_open() && size > offset) {
                Address fileSize = file.tellg();
                Address readSize = std::min(fileSize, size - offset);

                if(readSize > 0) {
                    file.seekg(0, std::ios::beg);
                    auto ptr = reinterpret_cast<char*>(mem.get() + offset);
                    file.read(ptr, readSize);
                }

                file.close();
                return true;
            }

            return false;
        }

        /**
         * Save all data in emulator memory to a binary file.
         *
         * @param path The path of the file to write to. Note that if a file already exists at this path, it will be
         *        overwritten.
         * @return Whether the file could be opened successfully or not.
         */
        bool saveToFile(std::string path) const {
            std::ofstream file;

            file.open(path,
                      std::ios::out | // Output/write mode.
                      std::ios::binary | // Binary mode.
                      std::ios::trunc); // Overwrite existing file contents should it already exist.

            if(file.is_open()) {
                auto ptr = reinterpret_cast<char*>(mem.get());
                file.write(ptr, size);

                file.close();
                return true;
            }

            return false;
        }

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