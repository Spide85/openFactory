#pragma once

#include "key.hpp"

#include <memory>
#include <string_view>

namespace spide2d {

struct hotkey_registry {
    /**
     * @brief Registers a global hotkey.
     * **Example usage:**
     *
     *     register_hotkey("close_window", key::create_from_human_readable("Escape"));
     */
    void register_hotkey(std::string_view action, key mapped_key);
};

}  // namespace spide2d