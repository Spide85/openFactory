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
#include <functional>

namespace spide2d::sdl {

struct sdl_exception : std::runtime_error {
    sdl_exception(std::string what) : std::runtime_error(what + ": " + SDL_GetError()) {}
};

namespace asio   = boost::asio;
using work_guard = asio::executor_work_guard<asio::io_context::executor_type>;

static asio::io_context                                       gl_context;
static asio::io_context                                       worker_context;
static std::thread                                            gl_thread;
static std::map<SDL_Window *, std::reference_wrapper<window>> sdl_windows;
static bool                                                   stop_mainloop {false};
work_guard worker_work_guard_ {asio::make_work_guard(worker_context)};  //!< io context will run w/o work

void start_gl_thread();
void dispatch_sdl_window_events(const SDL_WindowEvent &event);
void dispatch_sdl_events();
void queue_mainloop_work();

/**
 * @brief Pimpl class that handles all window specific stuff.
 */
class window::impl {
    SDL_Window *         sdl_window_ {nullptr};
    static SDL_GLContext sdl_gl_context_;
    window &             parent_;

public:
    impl(window &parent) : parent_(parent) {
        asio::post(gl_context, [this]() { init_sdl(); });
        asio::post(gl_context, [this]() { create_window(); });
        start_gl_thread();
    }

    ~impl() {
        worker_work_guard_.reset();

        // TODO: Cleanup
    }

    void init_sdl() {
        static auto alreadyInited {false};
        if (alreadyInited)
            return;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw sdl_exception("Could not initialize graphics");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(1);

        alreadyInited = true;
    }

    void create_window() {
        sdl_window_ = SDL_CreateWindow(
            "OpenFactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL);

        if (!sdl_window_)
            throw sdl_exception("Cannot open window");

        sdl_windows.emplace(sdl_window_, parent_);

        if (!sdl_gl_context_)
            sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
        if (!sdl_gl_context_)
            throw sdl_exception("Cannot create OpenGL context");

        gladLoadGLLoader(SDL_GL_GetProcAddress);
        spdlog::info("Vendor:   {}", glGetString(GL_VENDOR));
        spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
        spdlog::info("Version:  {}", glGetString(GL_VERSION));
    }

    void swap() const {
        SDL_GL_MakeCurrent(sdl_window_, sdl_gl_context_);
        glClearColor(1.0, 0.0, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(sdl_window_);
    }
};

SDL_GLContext window::impl::sdl_gl_context_ {nullptr};

/***********************************************************************************************************************
 * Free functions to do SDL/GL handling that is not associated with a class. All of them should be executed in the     *
 * gl_context only! (Only exception is start_gl_thread of course)                                                      *
 **********************************************************************************************************************/

void start_gl_thread() {
    if (gl_thread.joinable())
        return;
    gl_thread = std::thread([]() {
        spdlog::trace("Starting gl worker thread.");
        gl_context.run();
    });
}

void dispatch_sdl_window_events(const SDL_WindowEvent &event) {
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

/**
 * @brief Dispatches all events from SDL. Must run in gl thread.
 */
void dispatch_sdl_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: stop_mainloop = true; break;
            }
        case SDL_WINDOWEVENT: dispatch_sdl_window_events(event.window);
        }
    }
}

void swap_all_windows() {
    for (const auto &window : sdl_windows) {
        window.second.get().swap();
    }
}

/**
 * @brief Adds all workpackages that should run in one mainloop-iteration.
 */
void queue_mainloop_work() {
    if (stop_mainloop) {
        gl_context.stop();
        worker_context.stop();
        return;
    }
    asio::post(gl_context, &dispatch_sdl_events);
    asio::post(gl_context, &swap_all_windows);
    asio::post(gl_context, &queue_mainloop_work);
}

/***********************************************************************************************************************
 * Public class to pimpl forwarding                                                                                    *
 **********************************************************************************************************************/

window::window() : spide2d::window(), impl_ {std::make_unique<impl>(*this)} {}
window::~window() = default;

[[nodiscard]] connection window::hotkey_pressed(std::string_view action, const hotkey_slot &callback) {}

[[nodiscard]] connection window::hotkey_released(std::string_view action, const hotkey_slot &callback) {}

void window::run_event_loop() {
    queue_mainloop_work();
    worker_context.run();
}

void window::swap() const {
    impl_->swap();
}

}  // namespace spide2d::sdl
