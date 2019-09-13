#include "sdlwindow.hpp"

#include <spdlog/spdlog.h>
// clang-format off
#include <glad/glad.h>
#include <SDL2/SDL.h>
// clang-format on
#include <stdexcept>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/executor_work_guard.hpp>

namespace spide2d {

namespace priv {
boost::asio::io_context gl_context, worker_context;
auto                    gl_work_guard = boost::asio::make_work_guard(gl_context);
// auto                    worker_work_guard = boost::asio::make_work_guard(worker_context);
}  // namespace priv

struct sdl_exception : std::runtime_error {
    sdl_exception(std::string what) : std::runtime_error(what + ": " + SDL_GetError()) {}
};

sdl_window::sdl_window() : window() {
    boost::asio::post(priv::gl_context, [this]() { init_sdl(); });
    start_gl_thread();

    // while (true) {
    boost::asio::post(priv::worker_context, [this]() { dispatch_sdl_events(); });
    //}
}

void sdl_window::start_gl_thread() {
    gl_thread_ = std::thread([]() {
        spdlog::trace("Starting gl worker thread.");
        priv::gl_context.run();
    });
}

void sdl_window::init_sdl() {
    static auto alreadyInited {false};
    if (alreadyInited)
        throw std::runtime_error("[window] SDL2 was already inited.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw sdl_exception("Could not initialize graphics");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto window =
        SDL_CreateWindow("OpenFactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_OPENGL);

    if (!window)
        throw sdl_exception("Cannot open window");

    auto context = SDL_GL_CreateContext(window);
    if (!context)
        throw sdl_exception("Cannot create OpenGL context");

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    spdlog::info("Vendor:   {}", glGetString(GL_VENDOR));
    spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
    spdlog::info("Version:  {}", glGetString(GL_VERSION));

    glClearColor(1.0, 0.0, 0.5, 1.0);

    auto running = true;
    while (running) {
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }
    alreadyInited = true;
}

/**
 * @brief Dispatches all events from SDL. Must run in gl thread.
 *
 */
void sdl_window::dispatch_sdl_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            // running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: break;  // running = false; break;
                }
            }
    }
}

}  // namespace spide2d
