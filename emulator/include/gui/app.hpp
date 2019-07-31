#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace gui {
    class App {
    public:
        App(std::string title, unsigned int width, unsigned int height);
        void run();

    protected:
        virtual bool loop() = 0;
        virtual void render();
        virtual void stop();

        sf::RenderWindow window;
    };

    class EmuApp : public App {
    public:
        EmuApp();

    protected:
        bool loop() override final;
        void render() override final;
        void stop() override final;

    private:
        sf::Clock clock;
    };
}