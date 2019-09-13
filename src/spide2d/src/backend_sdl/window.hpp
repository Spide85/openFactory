#pragma once
#include <spide2d/window.hpp>
#include <thread>

namespace spide2d::sdl {

struct window : public spide2d::window {
    window();

    [[nodiscard]] connection hotkey_pressed(std::string_view action, const hotkey_slot &callback) override;

    [[nodiscard]] connection hotkey_released(std::string_view action, const hotkey_slot &callback) override;

private:
    void init_sdl();
    void start_gl_thread();
    void dispatch_sdl_events();

    std::thread gl_thread_;
};

}  // namespace spide2d::sdl