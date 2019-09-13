#pragma once

#include "key.hpp"
#include "signals.hpp"

namespace spide2d {

struct window {
    using hotkey_signal = signal<void()>;
    using hotkey_slot   = hotkey_signal::slot_type;

    virtual ~window() = default;

    [[nodiscard]] static std::unique_ptr<window> create(unsigned int width, unsigned int height);

    [[nodiscard]] virtual connection hotkey_pressed(std::string_view action, const hotkey_slot &callback) = 0;

    [[nodiscard]] virtual connection hotkey_released(std::string_view action, const hotkey_slot &callback) = 0;

protected:
    window();
};

}  // namespace spide2d
