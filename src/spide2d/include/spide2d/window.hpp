#pragma once

#include "hotkey_registry.hpp"
#include "signals.hpp"

#include <Eigen/Core>

namespace spide2d {

enum class mouse_button : uint8_t { left, middle, right, extra1, extra2 };

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

    /**
     * @brief Stops the event loop. Already queued events will be finished.
     */
    virtual void stop_event_loop() = 0;

    /**
     * @brief Gives you the hotkey registry of this window.
     *
     * @return hotkey_registry& A reference to the hotkey_registry. Lifetime/Ownership is bound to this object.
     */
    virtual hotkey_registry &keyboard() = 0;

    /**
     * @brief Close signal called when the user wants to close the window (by clicking on the X button oder pressing
     * Command-Q on a Mac.)
     *
     * If you don't react, nothing will happen. In general this is the right point for "Do you really want to quit?"
     */
    signal<void()> close;

    /**
     * @brief Signal is emitted when the user moves the mouse across the window. Position is in UI pixel coordinates.
     */
    signal<void(Eigen::Vector2i)> mouse_move;

    /**
     * @brief Signal is emitted when the user presses a mouse-button. Position is in UI pixel coordinates.
     */
    signal<void(Eigen::Vector2i, mouse_button)> mouse_down;

    /**
     * @brief Signal is emitted when the user releases a mouse-button. Position is in UI pixel coordinates.
     */
    signal<void(Eigen::Vector2i, mouse_button)> mouse_up;

protected:
    window();
};

}  // namespace spide2d
