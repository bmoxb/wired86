#include "registerinput.hpp"

#include "logging.hpp"
#include "convert.hpp"

namespace gui {
    RegisterInput::RegisterInput(emu::cpu::reg::GeneralRegisters& regs, emu::cpu::reg::GeneralRegister regIndex)
    : registers(regs), index(regIndex), buffer{ 0, 0 } {}

    void RegisterInput::update() {
        auto label = " : " + registers.getAssemblyIdentifier(index);

        ImGui::InputScalarN(label.c_str(), ImGuiDataType_U8, buffer,
                            2, // Two components (separate input boxes for low and high of register).
                            nullptr, nullptr, format, flags);

        logging::info("Values: " + convert::toHexString(getBufferValue()));

        // TODO: Appropriately sync buffer with actual register value.
    }

    u16 RegisterInput::getBufferValue() {
        u8 low = buffer[1],
           high = buffer[0];

        return convert::createWordFromBytes(low, high);
    }
}