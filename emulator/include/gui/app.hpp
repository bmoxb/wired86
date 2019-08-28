#pragma once

#include "primitives.hpp"
#include "registerinput.hpp"
#include "emu/cpu/intel8086.hpp"
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace gui {
    class App {
    public:
        App(std::string title, unsigned int width, unsigned int height, sf::Color colour = sf::Color::Black);
        void run();

    protected:
        virtual bool loop() = 0;
        virtual void stop();

        sf::RenderWindow window;

        const sf::Color clearColour;

    private:
        sf::Clock clock;
    };

    class EmuApp : public App {
    public:
        EmuApp();

    protected:
        bool loop() override final;

    private:
        emu::cpu::Intel8086 cpu;

        RegisterInput regInput;
    };
}