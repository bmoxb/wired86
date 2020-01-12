#include "emu/cpu/reg/registers8086.hpp"

#include "logging.hpp"

namespace emu::cpu::reg {
    constexpr auto UNKNOWN_INDEX = "<unknown register index>";

    std::string GeneralRegisters::getAssemblyIdentifier(GeneralRegister index) const {
        switch(index) {
        case AX_REGISTER: return "ax";
        case BX_REGISTER: return "bx";
        case CX_REGISTER: return "cx";
        case DX_REGISTER: return "dx";
        case SOURCE_INDEX: return "si";
        case DESTINATION_INDEX: return "di";
        case BASE_POINTER: return "bp";
        case STACK_POINTER: return "sp";
        }

        return UNKNOWN_INDEX;
    }

    std::string GeneralRegisters::getAssemblyIdentifier(GeneralRegister index, RegisterPart part) const {
        switch(part) {
        case LOW_BYTE:
            switch(index) {
            case AX_REGISTER: return "al";
            case BX_REGISTER: return "bl";
            case CX_REGISTER: return "cl";
            case DX_REGISTER: return "dl";
            default: break;
            }
            break;

        case HIGH_BYTE:
            switch(index) {
            case AX_REGISTER: return "ah";
            case BX_REGISTER: return "bh";
            case CX_REGISTER: return "ch";
            case DX_REGISTER: return "dh";
            default: break;
            }
            break;

        default: break;
        }

        return getAssemblyIdentifier(index);
    }

    std::string SegmentRegisters::getAssemblyIdentifier(SegmentRegister index) const {
        switch(index) {
        case CODE_SEGMENT: return "cs";
        case DATA_SEGMENT: return "ds";
        case EXTRA_SEGMENT: return "es";
        case STACK_SEGMENT: return "ss";
        }

        return UNKNOWN_INDEX;
    }

    std::string Flags::getAssemblyIdentifier(Flag) const {
        return "flags";
    }
}
