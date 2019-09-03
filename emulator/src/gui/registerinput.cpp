#include "registerinput.hpp"

#include "logging.hpp"
#include "convert.hpp"

namespace gui {
    RegisterInput::RegisterInput(emu::cpu::reg::GeneralRegisters& regs, emu::cpu::reg::GeneralRegister regIndex)
    : registers(regs), index(regIndex) {}

    void RegisterInput::update() {
        buffer = registers.get(index); // Update buffer with the register value.

        std::string label = registers.getAssemblyIdentifier(index) + ": " + convert::toHexString(registers.get(index)) + "\n"
                          + registers.getAssemblyIdentifier(index, emu::cpu::reg::LOW_BYTE) + ": " + convert::toHexString(registers.getLow(index)) + "\n"
                          + registers.getAssemblyIdentifier(index, emu::cpu::reg::HIGH_BYTE) + ": " + convert::toHexString(registers.getHigh(index));

        ImGui::InputScalar(label.c_str(), ImGuiDataType_U16, &buffer, nullptr, nullptr, format, flags);

        registers.set(index, buffer); // Update register value with entered value.
    }
}