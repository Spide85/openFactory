#pragma once
#include <experimental/propagate_const>
#include <spide2d/window.hpp>

namespace spide2d::sdl {

struct window : public spide2d::window {
    window();
    virtual ~window();

    [[nodiscard]] connection hotkey_pressed(std::string_view action, const hotkey_slot &callback) override;

    [[nodiscard]] connection hotkey_released(std::string_view action, const hotkey_slot &callback) override;

private:
    class impl;
    std::experimental::propagate_const<std::unique_ptr<impl>>
        impl_;  //!< see https://en.cppreference.com/w/cpp/language/pimpl
};

}  // namespace spide2d::sdl