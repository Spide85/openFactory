#include "window.hpp"

#include <spdlog/spdlog.h>
// clang-format off
#include <glad/glad.h>
#include <SDL2/SDL.h>
// clang-format on
#include <stdexcept>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/executor_work_guard.hpp>

namespace spide2d::sdl {

struct sdl_exception : std::runtime_error {
    sdl_exception(std::string what) : std::runtime_error(what + ": " + SDL_GetError()) {}
};

namespace asio   = boost::asio;
using work_guard = asio::executor_work_guard<asio::io_context::executor_type>;

window::window() : spide2d::window(), impl_ {std::make_unique<impl>()} {}
window::~window() = default;

[[nodiscard]] connection window::hotkey_pressed(std::string_view action, const hotkey_slot &callback) {}

[[nodiscard]] connection window::hotkey_released(std::string_view action, const hotkey_slot &callback) {}

class window::impl {
    asio::io_context gl_context_;
    asio::io_context worker_context_;
    work_guard       gl_work_guard_ {asio::make_work_guard(gl_context_)};
    std::thread      gl_thread_;
    bool             run_mainloop_ {true};

    SDL_Window *sdl_window_ {nullptr};

public:
    impl() {
        asio::post(gl_context_, [this]() { init_sdl(); });
        start_gl_thread();
        queue_mainloop_work();
    }

    ~impl() {
        gl_work_guard_.reset();
        gl_thread_.join();
    }

    /**
     * @brief Dispatches all events from SDL. Must run in gl thread.
     */
    void dispatch_sdl_events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                run_mainloop_ = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: run_mainloop_ = false; break;
                }
            }
        }
    }

    void init_sdl() {
        static auto alreadyInited {false};
        if (alreadyInited)
            throw std::runtime_error("[window] SDL2 was already inited.");

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw sdl_exception("Could not initialize graphics");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Refactor below (could be called multiple times)
        sdl_window_ = SDL_CreateWindow(
            "OpenFactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL);

        if (!sdl_window_)
            throw sdl_exception("Cannot open window");

        auto context = SDL_GL_CreateContext(sdl_window_);
        if (!context)
            throw sdl_exception("Cannot create OpenGL context");

        gladLoadGLLoader(SDL_GL_GetProcAddress);
        spdlog::info("Vendor:   {}", glGetString(GL_VENDOR));
        spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
        spdlog::info("Version:  {}", glGetString(GL_VERSION));

        glClearColor(1.0, 0.0, 0.5, 1.0);

        alreadyInited = true;
    }

    /**
     * @brief Adds all workpackages that should run in one mainloop-iteration.
     */
    void queue_mainloop_work() {
        if (!run_mainloop_)
            return;
        asio::post(gl_context_, [this]() { dispatch_sdl_events(); });
        asio::post(gl_context_, [this]() { swap(); });
    }

    void start_gl_thread() {
        gl_thread_ = std::thread([this]() {
            spdlog::trace("Starting gl worker thread.");
            gl_context_.run();
        });
    }

    void swap() {
        SDL_GL_SwapWindow(sdl_window_);
        glClear(GL_COLOR_BUFFER_BIT);
        queue_mainloop_work();
    }
};

}  // namespace spide2d::sdl
