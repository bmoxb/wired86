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

    bool RegisterIndex::operator==(const RegisterIndex& index) const {
        return name == index.name; // TODO: This is an efficient method of comparison that needs replacing.
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

    const GeneralIndex GeneralIndex::ax("AX", "AL", "AH"),
                       GeneralIndex::bx("BX", "BL", "BH"),
                       GeneralIndex::cx("CX", "CL", "CH"),
                       GeneralIndex::dx("DX", "DL", "DH");

    const IndexIndex   IndexIndex::source("SI"),
                       IndexIndex::destination("DI"),
                       IndexIndex::base("BP"),
                       IndexIndex::stack("SP");

    const SegmentIndex SegmentIndex::code("CS"),
                       SegmentIndex::data("DS"),
                       SegmentIndex::extra("ES"),
                       SegmentIndex::stack("SS");
}