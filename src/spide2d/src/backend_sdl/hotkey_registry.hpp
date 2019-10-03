#include <SDL2/SDL_keyboard.h>
#include <SDL_events.h>
#include <boost/asio/io_context.hpp>
#include <spide2d/hotkey_registry.hpp>
#include <unordered_map>

namespace spide2d::sdl {
using io_context = boost::asio::io_context;

struct hotkey_hasher {
    std::size_t operator()(const hotkey &k) const {
        return std::hash<std::string>()(k.key_name_) << 3 | k.alt_ << 2 | k.shift_ << 1 | k.strg_;
    }
};

struct hotkey_registry : spide2d::hotkey_registry {
    virtual ~hotkey_registry() = default;

    void register_hotkey(std::string action, hotkey mapped_key) override;

    [[nodiscard]] hotkey_signal &pressed(const std::string &action) override;

    [[nodiscard]] hotkey_signal &released(const std::string &action) override;

    void dispatch_sdl_event(const SDL_KeyboardEvent &event, io_context &context);

private:
    using action_to_hotkeys_map = std::unordered_map<std::string, hotkey_signal>;

    [[nodiscard]] static hotkey translate(SDL_Keysym k);

    /**
     * @brief  Stores an association from a hotkey to a vector of actions.
     * Main use-case is an incoming SDL_Keysym (created from libsdl as systemcallback for a key down/up event) that
     * we need to translate to a bunch of actions. These actions will be sent as signals.
     * So we have many lookups and only few (mainly on program startup) inserts, this means a unordered map will serve
     * good.
     */
    std::unordered_multimap<hotkey, std::string, hotkey_hasher> hotkeys_;

    action_to_hotkeys_map pressed_signals_;
    action_to_hotkeys_map released_signals_;

    void broadcast_hotkey(action_to_hotkeys_map &target, hotkey hk, io_context &context);
};

}  // namespace spide2d::sdl
