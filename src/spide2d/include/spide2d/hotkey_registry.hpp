#pragma once

#include "hotkey.hpp"
#include "signals.hpp"

#include <memory>
#include <string>

namespace spide2d {

struct hotkey_registry {
    using hotkey_signal = signal<void()>;  //!< The function definition of a hotkey-signal.

    virtual ~hotkey_registry() = default;

    /**
     * **Example usage:**
     *
     *     register_hotkey("close_window", hotkey{"Escape"});
     */
    virtual void register_hotkey(std::string action, hotkey mapped_key) = 0;

    [[nodiscard]] virtual hotkey_signal &pressed(const std::string &action) = 0;

    [[nodiscard]] virtual hotkey_signal &released(const std::string &action) = 0;
};

}  // namespace spide2d