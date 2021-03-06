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

window::~window() {
    SDL_DestroyWindow(sdl_window_);
    SDL_Quit();
}

/**
 * @brief Dispatches all events from SDL. Must run in gl thread.
 */
void window::dispatch_sdl_events() {
    SDL_Event event;
    ui_renderer_.start_input_handling();
    while (SDL_PollEvent(&event)) {
        ui_renderer_.handle_event(event);
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP: hotkey_registry_.dispatch_sdl_event(event.key, logic_context_); break;
        case SDL_MOUSEMOTION:
            logic_context_.post([this, event]() { mouse_move({event.motion.x, event.motion.y}); });
            break;
        case SDL_MOUSEBUTTONDOWN:
            logic_context_.post([this, event]() {
                mouse_down({event.button.x, event.button.y}, static_cast<mouse_button>(event.button.button));
            });
            break;
        case SDL_MOUSEBUTTONUP:
            logic_context_.post([this, event]() {
                mouse_up({event.button.x, event.button.y}, static_cast<mouse_button>(event.button.button));
            });
            break;

        case SDL_QUIT: logic_context_.post([this]() { close(); }); break;
        }
    }
    ui_renderer_.finish_input_handling();
}

void window::init_sdl() {
    static std::mutex function_mutex;
    static auto       sdl_already_inited {false};

    // Locking the function to prevent multple instances initing at the same time.
    std::lock_guard<std::mutex> lock(function_mutex);
    if (sdl_already_inited)
        throw std::runtime_error("Multiple concurrent windows are not supported.");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
        throw sdl_exception("Could not initialize graphics");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    sdl_window_ = SDL_CreateWindow("OpenFactory",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   1200,
                                   800,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

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

    ui_renderer_.set_sdl_window(sdl_window_);

    sdl_already_inited = true;
}

void window::queue_mainloop_work() {
    boost::asio::post(gl_context_, [this]() { dispatch_sdl_events(); });
    boost::asio::post(gl_context_, [this]() { ui_renderer_.render(); });
    boost::asio::post(gl_context_, [this]() { swap(); });
    boost::asio::post(gl_context_, [this]() { queue_mainloop_work(); });
}

void window::run_event_loop() {
    boost::asio::post(gl_context_, [this]() { queue_mainloop_work(); });
    logic_context_.run();
}

void window::stop_event_loop() {
    gl_context_.stop();
    logic_context_.stop();
}

spide2d::hotkey_registry &window::keyboard() {
    return hotkey_registry_;
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
    SDL_GL_SwapWindow(sdl_window_);
    glClear(GL_COLOR_BUFFER_BIT);
}

}  // namespace spide2d::sdl
