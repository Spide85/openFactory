#include "game.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("C++ Standard used: {}", __cplusplus);

    try {
        auto window  = spide2d::window::create(800, 600);
        auto window2 = spide2d::window::create(800, 600);
        spide2d::window::run_event_loop();
        spdlog::info("Ende");
    } catch (std::exception &e) {
        spdlog::critical("Critical exception: {}, type: {}", e.what(), typeid(e).name());
    }
}
