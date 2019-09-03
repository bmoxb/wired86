#pragma once

#include "primitives.hpp"
#include "emu/cpu/reg/registers8086.hpp"
#include <imgui.h>

namespace gui {
    class RegisterInput {
    public:
        RegisterInput(emu::cpu::reg::GeneralRegisters& regs, emu::cpu::reg::GeneralRegister regIndex);

        void update();

    private:
        emu::cpu::reg::GeneralRegisters& registers;
        emu::cpu::reg::GeneralRegister index;

        u8 buffer[2]; // low, high

        constexpr static auto flags = ImGuiInputTextFlags_CharsHexadecimal
                                    | ImGuiInputTextFlags_CharsUppercase
                                    | ImGuiInputTextFlags_CharsNoBlank;

        constexpr static auto format = "%X";
    };
}