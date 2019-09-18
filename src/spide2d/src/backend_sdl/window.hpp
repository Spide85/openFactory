#pragma once
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
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

    [[nodiscard]] connection hotkey_pressed(std::string_view action, const hotkey_slot &callback) override;

    [[nodiscard]] connection hotkey_released(std::string_view action, const hotkey_slot &callback) override;

    void run_event_loop() override;

protected:
private:
    io_context  gl_context_;
    std::thread gl_thread_;
    io_context  worker_context_;
    work_guard  worker_work_guard_ {boost::asio::make_work_guard(worker_context_)};

    SDL_Window *  sdl_window_ {nullptr};
    SDL_GLContext sdl_gl_context_;

    bool stop_mainloop_ {false};

    void dispatch_sdl_events();
    void dispatch_sdl_keyboard_events(const SDL_KeyboardEvent &event);
    void dispatch_sdl_window_events(const SDL_WindowEvent &event);
    void init_sdl();
    void queue_mainloop_work();
    void start_gl_thread();
    void swap();
};

}  // namespace spide2d::sdl