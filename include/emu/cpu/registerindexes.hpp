#pragma once

#include <string>

namespace emu::cpu {
    enum RegisterPart { FULL_WORD, LOW_BYTE, HIGH_BYTE };

    /**
     * Class used to indicate which register is required within a collect of registers. Should be used somewhat like a
     * Java-style enum by extending this call and then having each enum item as a static instance of that same class.
     */
    class RegisterIndex {
    protected:
        RegisterIndex(std::string assembly);

    public:
        // TODO: Document! Fix docs for other toAssembly methods.
        static std::string toAssembly(std::string indexName, bool intelSyntax = true);

        /**
         * Converts this register index to its name in x86 assembly. When using Intel syntax then the short-hand name
         * is simply returned (e.g. 'ax') while using AT&T syntax '%' is prepended to the name (e.g. '%ax').
         *
         * @param intelSyntax Use Intel syntax when true and AT&T syntax when false.
         * @return Assembly representation of this register index.
         */
        std::string toAssembly(bool intelSyntax = true);

    private:
        const std::string assemblyName;
    };

    class RegisterIndexLowHigh : public RegisterIndex {
    protected:
        RegisterIndexLowHigh(std::string assembly, std::string assemblyLow, std::string assemblyHigh);

    public:
        std::string toAssembly(RegisterPart part, bool intelSyntax = true);

    private:
        std::string assemblyLowName, assemblyHighName;
    };

    class GeneralIndex : public RegisterIndexLowHigh {
    public: static GeneralIndex AX, BX, CX, DX;
    };

    class SegmentIndex : public RegisterIndex {
    public: static SegmentIndex CODE, DATA, EXTRA, STACK;
    };
}