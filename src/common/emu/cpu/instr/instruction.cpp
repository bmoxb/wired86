#include "emu/cpu/instr/instruction.hpp"

#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode)
    : identifier(instrIdentifier), opcode(instrOpcode) {}

    std::string Instruction::getRawDataString(std::string separator) const {
        auto raw = getRawData();
        std::string str;

        for(unsigned int i = 0; i < raw.size() - 1; i++)
            str += convert::toHexString(raw[i]) + separator; // Add all elements (except the final one) with separator.
        
        str += convert::toHexString(raw[raw.size() - 1]); // Add final element (no separator).

        return str;
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }
}