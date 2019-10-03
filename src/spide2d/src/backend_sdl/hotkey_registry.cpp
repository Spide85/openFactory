#include "hotkey_registry.hpp"

namespace spide2d::sdl {

void hotkey_registry::register_hotkey(std::string action, hotkey mapped_key) {
    hotkeys_.emplace(mapped_key, action);
}

hotkey_registry::hotkey_signal &hotkey_registry::pressed(const std::string &action) {
    return pressed_signals_.at(action);
}
hotkey_registry::hotkey_signal &hotkey_registry::released(const std::string &action) {
    return released_signals_.at(action);
}

hotkey hotkey_registry::translate(SDL_Keysym k) {
    hotkey hk;
    hk.alt_      = (k.mod & KMOD_LALT) | (k.mod & KMOD_RALT);
    hk.shift_    = (k.mod & KMOD_LSHIFT) | (k.mod & KMOD_RSHIFT);
    hk.strg_     = (k.mod & KMOD_LCTRL) | (k.mod & KMOD_RCTRL);
    hk.key_name_ = SDL_GetKeyName(k.sym);
    return hk;
}

}  // namespace spide2d::sdl
