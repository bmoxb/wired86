#include "emu/cpu/registerindexes.hpp"

namespace emu::cpu {
    RegisterIndex::RegisterIndex(std::string assembly) : assemblyName(assembly) {}

    std::string RegisterIndex::toAssembly(std::string indexName, bool intelSyntax) {
        if(intelSyntax) return indexName;
        else return "%" + indexName;
    }

    std::string RegisterIndex::toAssembly(bool intelSyntax) {
        return toAssembly(assemblyName, intelSyntax);
    }

    RegisterIndexLowHigh::RegisterIndexLowHigh(std::string assembly, std::string assemblyLow, std::string assemblyHigh)
    : RegisterIndex(assembly), assemblyLowName(assemblyLow), assemblyHighName(assemblyHigh) {}

    std::string RegisterIndexLowHigh::toAssembly(RegisterPart part, bool intelSyntax) {
        switch(part) {
        case FULL_WORD: return toAssembly(assemblyName, intelSyntax);
        case LOW_BYTE: return toAssembly(assemblyLowName, intelSyntax);
        case HIGH_BYTE: return toAssembly(assemblyHighName, intelSyntax);
        }
    }

    GeneralIndex GeneralIndex::AX("AX", "AL", "AH");
}