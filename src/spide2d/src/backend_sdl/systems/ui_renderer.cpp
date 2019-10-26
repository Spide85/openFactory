#include <spide2d/systems/ui_renderer.hpp>
// clang-format off
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#define NK_IMPLEMENTATION
#include <cstdarg> // Needed for nuklear
#include <SDL2/SDL.h>
#include <nuklear.h>
#include <demo/sdl_opengl3/nuklear_sdl_gl3.h> // Maybe this needs a port/refactor, but for now it works good
#include <demo/node_editor.c>
// clang-format on
#include <spdlog/spdlog.h>

namespace spide2d {

ui_renderer::ui_renderer(SDL_Window *sdl_window) {
    set_sdl_window(sdl_window);
}

void ui_renderer::set_sdl_window(SDL_Window *sdl_window) noexcept {
    sdl_window_ = sdl_window;
    if (sdl_window_)
        ctx_ = nk_sdl_init(sdl_window_);
    else
        return;

    {
        nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        auto *droid = nk_font_atlas_add_from_file(atlas, "../share/openFactory/extra_font/DroidSans.ttf", 14, 0);
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16,
         * 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf",
         * 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13,
         * 0);*/
        if (!droid)
            return;
        nk_sdl_font_stash_end();
        nk_style_load_all_cursors(ctx_, atlas->cursors);
        nk_style_set_font(ctx_, &droid->handle);
    }
}

void ui_renderer::start_input_handling() noexcept {
    nk_input_begin(ctx_);
}

void ui_renderer::handle_event(SDL_Event &e) noexcept {
    nk_sdl_handle_event(&e);
}

void ui_renderer::finish_input_handling() noexcept {
    nk_input_end(ctx_);
}

void ui_renderer::render() noexcept {
    if (nk_begin(ctx_,
                 "Demo",
                 nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        enum { EASY, HARD };
        static int       op       = EASY;
        static int       property = 20;
        struct nk_colorf bg;
        bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

        nk_layout_row_static(ctx_, 30, 80, 1);
        if (nk_button_label(ctx_, "button"))
            printf("button pressed!\n");
        nk_layout_row_dynamic(ctx_, 30, 2);
        if (nk_option_label(ctx_, "easy", op == EASY))
            op = EASY;
        if (nk_option_label(ctx_, "hard", op == HARD))
            op = HARD;
        nk_layout_row_dynamic(ctx_, 22, 1);
        nk_property_int(ctx_, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(ctx_, 20, 1);
        nk_label(ctx_, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx_, 25, 1);
        if (nk_combo_begin_color(ctx_, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx_), 400))) {
            nk_layout_row_dynamic(ctx_, 120, 1);
            bg = nk_color_picker(ctx_, bg, NK_RGBA);
            nk_layout_row_dynamic(ctx_, 25, 1);
            bg.r = nk_propertyf(ctx_, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
            bg.g = nk_propertyf(ctx_, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
            bg.b = nk_propertyf(ctx_, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
            bg.a = nk_propertyf(ctx_, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
            nk_combo_end(ctx_);
        }
    }
    nk_end(ctx_);

    node_editor(ctx_);

    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

}  // namespace spide2d