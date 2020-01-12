#pragma once

#include <string>

namespace assembly {
    enum NumericalRepresentation {
        HEX_REPRESENTATION,
        BINARY_REPRESENTATION,
        DENARY_REPRESENTATION
    };

    enum NumericalStyle {
        WITH_PREFIX,
        WITH_SUFFIX,
        WITHOUT_SUFFIX_OR_PREFIX
    };

    struct Style {
        NumericalRepresentation numericalRepresentation;
        NumericalStyle numericalStyle;

        std::string argumentSeparator = ", ";

        std::string displacementBegin = "[";
        std::string displacementEnd = "]";
        std::string displacementAdd = " + ";

        std::string hexPrefix = "0x";
        std::string hexSuffix = "h";

        std::string binaryPrefix = "0b";
        std::string binarySuffix = "b";
    };
}
