#pragma once

#include "signals.hpp"

namespace spide2d {

/**
 * @brief A window represents a native drawing area on screen. Most native APIs handle input with this window:
 * It's only possible to get input if the window has focus or is on fullscreen mode.
 * If the window is deleted, the render-context is deleted too. That means all resources are deleten from VRAM.
 */
struct window {
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

    /**
     * @brief Starts the event loop.
     */
    virtual void run_event_loop() = 0;

protected:
    window();
};

}  // namespace spide2d
