#include "game.hpp"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("C++ Standard used: {}", __cplusplus);

    try {
        // spide2d::hotkeys hotkeys;

        // of::game g(std::make_unique<spide2d::sdl_window>(800, 600, hotkeys));
    } catch (std::exception &e) {
        spdlog::critical("Critical exception: {}, type: {}", e.what(), typeid(e).name());
    }
}
