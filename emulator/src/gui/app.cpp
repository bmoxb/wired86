#include "app.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Window/Event.hpp>

namespace gui {
    App::App(std::string title, unsigned int width, unsigned int height)
    : window(sf::VideoMode(width, height), title) {}

    void App::run() {
        bool cont;

        while(window.isOpen()) {
            cont = loop();

            window.clear(sf::Color(255, 0, 0, 255));
            render();

            if(!cont) stop();
        }
    }

    void App::render() {
        window.display();
    }

    void App::stop() {
        window.close();
    }



    EmuApp::EmuApp() : App("Wired86", 800, 600) {
        ImGui::SFML::Init(window);
    }

    bool EmuApp::loop() {
        sf::Event event;
        
        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed) return false;
        }

        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Window");
        ImGui::Button("Click");
        ImGui::End();

        return true;
    }

    void EmuApp::render() {
        ImGui::SFML::Render(window);
        App::render();
    }

    void EmuApp::stop() {
        App::stop();
        ImGui::SFML::Shutdown();
    }
}