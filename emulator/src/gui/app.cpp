#include "app.hpp"

#include "logging.hpp"
#include "convert.hpp"
#include "emu/cpu/reg/registers8086.hpp"
#include <vector>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Window/Event.hpp>

namespace gui {
    App::App(std::string title, unsigned int width, unsigned int height, sf::Color colour)
    : window(sf::VideoMode(width, height), title), clearColour(colour) {
        ImGui::SFML::Init(window);
    }

    void App::run() {
        bool cont;
        sf::Event event;

        while(window.isOpen()) {        
            while(window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if(event.type == sf::Event::Closed) stop();
            }
            
            ImGui::SFML::Update(window, clock.restart());

            cont = loop();

            window.clear(clearColour);
            ImGui::SFML::Render(window);
            window.display();

            if(!cont) stop();
        }
    }

    void App::stop() {
        window.close();
        ImGui::SFML::Shutdown();
    }



    EmuApp::EmuApp()
    : App("Wired86", 800, 600, sf::Color(135, 206, 250, 255)),
      regInput(cpu.generalRegisters, emu::cpu::reg::AX_REGISTER) {}

    bool EmuApp::loop() {
        ImGui::Begin("Registers (high:low)");
        regInput.update();
        ImGui::End();

        return true;
    }
}