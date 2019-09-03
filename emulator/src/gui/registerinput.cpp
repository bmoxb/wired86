#include "registerinput.hpp"

#include "logging.hpp"
#include "convert.hpp"

namespace gui {
    RegisterInput::RegisterInput(emu::cpu::reg::GeneralRegisters& regs, emu::cpu::reg::GeneralRegister regIndex)
    : registers(regs), index(regIndex) {}

    void RegisterInput::update() {
        buffer[0] = registers.getLow(index);
        buffer[1] = registers.getHigh(index);

        ImGui::InputScalar((": " + registers.getAssemblyIdentifier(index)).c_str(),
                           ImGuiDataType_U16, &buffer, nullptr, nullptr, format, flags);

        ImGui::InputScalar((": " + registers.getAssemblyIdentifier(index, emu::cpu::reg::LOW_BYTE)).c_str(),
                           ImGuiDataType_U8, &buffer[0], nullptr, nullptr, format, flags);
        ImGui::InputScalar((": " + registers.getAssemblyIdentifier(index, emu::cpu::reg::HIGH_BYTE)).c_str(),
                           ImGuiDataType_U8, &buffer[1], nullptr, nullptr, format, flags);

        //registers.set(index, buffer); // Update register value with entered value.
        registers.setLow(index, buffer[0]);
        registers.setHigh(index, buffer[1]);
    }
}