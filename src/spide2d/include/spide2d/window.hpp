#pragma once

#include "key.hpp"
#include "signals.hpp"

namespace spide2d {

/**
 * @brief A window represents a native drawing area on screen. Most native APIs handle input with this window:
 * It's only possible to get input if the window has focus or is on fullscreen mode.
 * If the window is deleted, the render-context is deleted too. That means all resources are deleten from VRAM.
 */
struct window {
    using hotkey_signal = signal<void()>;            //!< The function definition of a hotkey-signal.
    using hotkey_slot   = hotkey_signal::slot_type;  //!< Just a shortcut for a receiver slot of a hotkey.

    virtual ~window() = default;

    /**
     * @brief Factory function for creating a window.
     * Maybe in future this is the point for offering different implementations (Allegro/SDL/GLFW/WinAPI/DX etc)
     *
     * @param width Needs to be moved to some settings-construct
     * @param height
     * @return std::unique_ptr<window> The created window.
     */
    [[nodiscard]] static std::unique_ptr<window> create(unsigned int width, unsigned int height);

    [[nodiscard]] virtual connection hotkey_pressed(std::string_view action, const hotkey_slot &callback) = 0;

    [[nodiscard]] virtual connection hotkey_released(std::string_view action, const hotkey_slot &callback) = 0;

    /**
     * @brief Starts the event loop. No matter how many windows you have, u will only start one main-loop.
     *
     */
    static void run_event_loop();

protected:
    window();
};

}  // namespace spide2d
