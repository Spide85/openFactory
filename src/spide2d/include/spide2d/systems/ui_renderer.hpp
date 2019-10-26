#pragma once

// clang-format off
#include <glad/glad.h>
#include <SDL2/SDL.h>
// clang-format on

struct nk_context;

namespace spide2d {

/**
 * @brief All functions needs to be called from gl context thread.
 *
 */
struct ui_renderer {
    /**
     * @brief Construct a new ui renderer object
     *
     * @param sdl_window ui_renderer does not take the ownership but requires that it stays valid as long as this object
     * exists.
     */
    ui_renderer(SDL_Window *sdl_window = nullptr);

    void set_sdl_window(SDL_Window *sdl_window) noexcept;

    void start_input_handling() noexcept;
    void handle_event(SDL_Event &e) noexcept;
    void finish_input_handling() noexcept;

    void render() noexcept;

private:
    SDL_Window *sdl_window_ {nullptr};
    nk_context *ctx_ {nullptr};
};

}  // namespace spide2d
