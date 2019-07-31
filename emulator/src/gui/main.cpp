#include <gtkmm.h>
#include "window.hpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "wiredsound.wired86");

    Window window;

    return app->run(window);
}