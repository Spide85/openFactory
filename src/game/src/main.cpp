#include "game.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

int main(int, char *[]) {
    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("C++ Standard used: {}", __cplusplus);

    try {
        auto window = spide2d::window::create(800, 600);
        window->run_event_loop();
        spdlog::info("Ende");
    } catch (std::exception &e) {
        spdlog::critical("Critical exception: {}, type: {}", e.what(), typeid(e).name());
    }
}
