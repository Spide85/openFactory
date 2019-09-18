#include "window.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <functional>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace spide2d::sdl {

struct sdl_exception : std::runtime_error {
    sdl_exception(std::string what) : std::runtime_error(what + ": " + SDL_GetError()) {}
};

window::window() : spide2d::window() {
    boost::asio::post(gl_context_, [this]() { init_sdl(); });
    start_gl_thread();
}

window::~window() = default;

[[nodiscard]] connection window::hotkey_pressed(std::string_view action, const hotkey_slot &callback) {}

[[nodiscard]] connection window::hotkey_released(std::string_view action, const hotkey_slot &callback) {}
/**
 * @brief Dispatches all events from SDL. Must run in gl thread.
 */
void window::dispatch_sdl_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: break;
        case SDL_KEYDOWN: dispatch_sdl_keyboard_events(event.key); break;
        case SDL_WINDOWEVENT: dispatch_sdl_window_events(event.window); break;
        }
    }
}

void window::dispatch_sdl_window_events(const SDL_WindowEvent &event) {
    switch (event.event) {
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        // if (SDL_GetWindowFromID(event.windowID) == sdl_window_)
        //    sdl_window_focus_ = true;
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        // if (SDL_GetWindowFromID(event.windowID) == sdl_window_)
        //    sdl_window_focus_ = false;
        break;
    }
}

void window::dispatch_sdl_keyboard_events(const SDL_KeyboardEvent &event) {
    switch (event.type) {
    case SDL_KEYDOWN: break;
    case SDL_KEYUP: break;
    }
}
void window::init_sdl() {
    static auto sdl_already_inited {false};
    if (sdl_already_inited)
        return;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw sdl_exception("Could not initialize graphics");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    sdl_window_ =
        SDL_CreateWindow("OpenFactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL);

    if (!sdl_window_)
        throw sdl_exception("Cannot open window");

    sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);

    if (!sdl_gl_context_)
        throw sdl_exception("Cannot create OpenGL context");

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    spdlog::info("Vendor:   {}", glGetString(GL_VENDOR));
    spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
    spdlog::info("Version:  {}", glGetString(GL_VERSION));
    glClearColor(1.0, 0.0, 0.5, 1.0);

    sdl_already_inited = true;
}

void window::queue_mainloop_work() {
    if (stop_mainloop_) {
        gl_context_.stop();
        worker_context_.stop();
        return;
    }
    boost::asio::post(gl_context_, [this]() { dispatch_sdl_events(); });
    boost::asio::post(gl_context_, [this]() { swap(); });
    boost::asio::post(gl_context_, [this]() { queue_mainloop_work(); });
}

void window::run_event_loop() {
    boost::asio::post(gl_context_, [this]() { queue_mainloop_work(); });
    worker_context_.run();
}

void window::start_gl_thread() {
    // Check if already a gl thread is running.
    if (gl_thread_.joinable())
        return;

    // Start a gl thread and execute the asio queue.
    gl_thread_ = std::thread([this]() {
        spdlog::trace("Starting gl thread.");
        gl_context_.run();
    });
}

void window::swap() {
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(sdl_window_);
}

}  // namespace spide2d::sdl
