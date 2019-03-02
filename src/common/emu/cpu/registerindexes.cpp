#include "emu/cpu/registerindexes.hpp"

namespace emu::cpu {
    RegisterIndex::RegisterIndex(std::string indexName, std::string indexDescription)
    : name(indexName), description(indexDescription) {}

    std::string RegisterIndex::toAssembly() const {
        return name;
    }

    std::string RegisterIndex::getInfo() const {
        if(description.size()) return name + " - " + description + " Register";
        else return name + " - Register";
    }

    RegisterIndexLowHigh::RegisterIndexLowHigh(std::string name, std::string description)
    : RegisterIndex(name, description), lowName(name + "(low)"), highName(name + "(high)") {}

    RegisterIndexLowHigh::RegisterIndexLowHigh(std::string name, std::string low, std::string high, std::string description)
    : RegisterIndex(name, description), lowName(low), highName(high) {}

    std::string RegisterIndexLowHigh::toAssembly(RegisterPart part) const {
        switch(part) {
        case LOW_BYTE: return lowName;
        case HIGH_BYTE: return highName;
        default: return toAssembly();
        }
    }

    const GeneralIndex GeneralIndex::AX("AX", "AL", "AH"),
                       GeneralIndex::BX("BX", "BL", "BH"),
                       GeneralIndex::CX("CX", "CL", "CH"),
                       GeneralIndex::DX("DX", "DL", "DH");

    const SegmentIndex SegmentIndex::CODE("CS"),
                       SegmentIndex::DATA("DS"),
                       SegmentIndex::EXTRA("ES"),
                       SegmentIndex::STACK("SS");
}