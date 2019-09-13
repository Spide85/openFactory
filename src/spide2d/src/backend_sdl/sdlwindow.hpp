#pragma once
#include <spide2d/window.hpp>
#include <thread>

namespace spide2d {

struct sdl_window : window {
    sdl_window();

private:
    void init_sdl();
    void start_gl_thread();
    void dispatch_sdl_events();

    std::thread gl_thread_;
};

}  // namespace spide2d