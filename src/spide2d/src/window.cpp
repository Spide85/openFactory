#include "backend_sdl/window.hpp"

#include <spide2d/window.hpp>

namespace spide2d {

window::window() {}

std::unique_ptr<window> window::create(unsigned int width, unsigned int height) {
    return std::make_unique<sdl::window>();
}

void window::run_event_loop() {
    // if (api == sdl) ...
    sdl::window::run_event_loop();
}

}  // namespace spide2d
