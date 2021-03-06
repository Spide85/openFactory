#include "backend_sdl/window.hpp"

#include <spide2d/window.hpp>

namespace spide2d {

window::window() {}

std::unique_ptr<window> window::create(unsigned int, unsigned int) {
    return std::make_unique<sdl::window>();
}

}  // namespace spide2d
