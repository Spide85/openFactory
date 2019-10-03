#include "game.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

int main(int, char *[]) {
    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("C++ Standard used: {}", __cplusplus);

    try {
        auto window = spide2d::window::create(800, 600);
        window->keyboard().register_hotkey("instant terminate", spide2d::hotkey {"Escape"});
        window->keyboard().pressed("instant terminate").connect([&window]() {
            window->stop_event_loop();
            spdlog::info("Eventloop stopped.");
        });
        window->close.connect([&window]() {
            window->stop_event_loop();
            spdlog::info("Eventloop stopped.");
        });
        window->mouse_move.connect([](Eigen::Vector2i v) { spdlog::trace("Mouse: {}, {}", v.x(), v.y()); });
        window->mouse_down.connect([](Eigen::Vector2i v, spide2d::mouse_button mb) {
            spdlog::info("Mousebutton {} down at {}, {}", static_cast<uint8_t>(mb), v.x(), v.y());
        });
        window->run_event_loop();
        spdlog::info("Main function terminated.");
    } catch (std::exception &e) {
        spdlog::critical("Critical exception: {}, type: {}", e.what(), typeid(e).name());
    }
}
