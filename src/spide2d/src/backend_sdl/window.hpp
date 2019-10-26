#pragma once
#include "hotkey_registry.hpp"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <spide2d/systems/ui_renderer.hpp>
#include <spide2d/window.hpp>
#include <thread>
// clang-format off
#include <glad/glad.h>
#include <SDL2/SDL.h>
// clang-format on

namespace spide2d::sdl {

using io_context = boost::asio::io_context;
using work_guard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

struct window : public spide2d::window {
    window();
    virtual ~window();

    void run_event_loop() override;

    void stop_event_loop() override;

    spide2d::hotkey_registry &keyboard() override;

protected:
private:
    io_context  gl_context_;
    std::thread gl_thread_;
    io_context  logic_context_;
    work_guard  logic_work_guard_ {boost::asio::make_work_guard(logic_context_)};

    SDL_Window *  sdl_window_ {nullptr};
    SDL_GLContext sdl_gl_context_;
    ui_renderer   ui_renderer_;

    hotkey_registry hotkey_registry_;

    void dispatch_sdl_events();
    void init_sdl();
    void queue_mainloop_work();
    void start_gl_thread();
    void swap();
};

}  // namespace spide2d::sdl