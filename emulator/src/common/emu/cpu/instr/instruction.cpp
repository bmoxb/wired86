#include "emu/cpu/instr/instruction.hpp"

#include <functional>
#include "emu/cpu/intel8086.hpp"
#include "logging.hpp"

namespace emu::cpu::instr {
    Instruction::Instruction(std::string instrIdentifier, Opcode instrOpcode)
    : identifier(instrIdentifier), opcode(instrOpcode) {}

    std::vector<u8> Instruction::getRawData() const {
        return { opcode.value }; // By default, only the opcode value is returned as raw data.
    }

    std::string Instruction::getRawDataString(std::string separator) const {
        std::vector<u8> raw = getRawData();
        std::function<std::string(u8)> convertFunction = [](u8 value) { return convert::toBinaryString<8>(value); };
        
        return convert::vectorToString(raw, convertFunction, separator);
    }

    OffsetAddr Instruction::getRawSize() const {
        std::size_t size = getRawData().size();
        return static_cast<OffsetAddr>(size);
    }



    InstructionTakingRegister::InstructionTakingRegister(std::string instrIdentifier, Opcode instrOpcode,
                                                         reg::GeneralRegister generalReg, reg::RegisterPart part)
    : Instruction(instrIdentifier, instrOpcode),
      registerIndex(generalReg), registerPart(part) {}

    std::string InstructionTakingRegister::toAssembly(const Intel8086& cpu) const {
        auto registerIdentifier = cpu.generalRegisters.getAssemblyIdentifier(registerIndex, registerPart);
        return identifier + " " + registerIdentifier;
    }



    InstructionTakingRegistersModRegRm::InstructionTakingRegistersModRegRm(std::string instrIdentifier,
                                                                           Opcode instrOpcode, ModRegRm instrModRegRm)
    : Instruction(instrIdentifier, instrOpcode),
      modRegRm(instrModRegRm) {}

    /*OffsetAddr InstructionTakingRegistersModRegRm::execute(Intel8086& cpu, OffsetAddr ip, Mem& memory,
                                                           reg::GeneralRegisters& generalRegisters,
                                                           reg::SegmentRegisters& segmentRegisters, reg::Flags& flags) {
        return ip + getRawSize();
    }*/

    std::vector<u8> InstructionTakingRegistersModRegRm::getRawData() const {
        return { opcode.value, modRegRm.value };
    }
}