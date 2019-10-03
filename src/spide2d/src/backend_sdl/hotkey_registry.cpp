#include "hotkey_registry.hpp"

namespace spide2d::sdl {

void hotkey_registry::register_hotkey(std::string action, hotkey mapped_key) {
    hotkeys_.emplace(mapped_key, action);
}

hotkey_registry::hotkey_signal &hotkey_registry::pressed(const std::string &action) {
    return pressed_signals_[action];
}
hotkey_registry::hotkey_signal &hotkey_registry::released(const std::string &action) {
    return released_signals_[action];
}

void hotkey_registry::dispatch_sdl_event(const SDL_KeyboardEvent &event, io_context &context) {
    action_to_hotkeys_map *target {nullptr};
    switch (event.type) {
    case SDL_KEYDOWN: target = &pressed_signals_; break;
    case SDL_KEYUP: target = &released_signals_; break;
    default: return;
    }
    broadcast_hotkey(*target, translate(event.keysym), context);
}

hotkey hotkey_registry::translate(SDL_Keysym k) {
    hotkey hk;
    hk.alt_      = (k.mod & KMOD_LALT) | (k.mod & KMOD_RALT);
    hk.shift_    = (k.mod & KMOD_LSHIFT) | (k.mod & KMOD_RSHIFT);
    hk.strg_     = (k.mod & KMOD_LCTRL) | (k.mod & KMOD_RCTRL);
    hk.key_name_ = SDL_GetKeyName(k.sym);
    return hk;
}

void hotkey_registry::broadcast_hotkey(action_to_hotkeys_map &target, hotkey hk, io_context &context) {
    // search all actions that are triggered with the key stroke
    auto triggered_actions = hotkeys_.equal_range(hk);

    // broadcast each of these actions
    for (auto it = triggered_actions.first; it != triggered_actions.second; ++it) {
        // check if somebody listens on this, if yes, we queue the signal invocation in the context (logic thread)
        auto tr_it = target.find(it->second);
        if (tr_it == target.end())
            continue;
        auto &triggered_signal = tr_it->second;
        context.post([&triggered_signal]() {
            triggered_signal();  // can be problematic if the signal is deleted in the meantime. But mutex and
                                 // shared_ptr are looking like a big overhead.
        });
    }
}

}  // namespace spide2d::sdl
