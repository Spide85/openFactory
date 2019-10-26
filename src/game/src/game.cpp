#include "game.hpp"

#include "ressources.hpp"

namespace of {

game::game(std::unique_ptr<spide2d::window> window) : window_(std::move(window)) {
    window_->keyboard().register_hotkey("instant terminate", spide2d::hotkey {"Escape"});
    window_->keyboard().pressed("instant terminate").connect([&window]() {
        window->stop_event_loop();
        spdlog::info("Eventloop stopped.");
    });

    load_mods();
    window_->run_event_loop();
}

void game::load_mods() {
    for (const auto mod : fs::directory_iterator(mods_dir())) {
        mods_.emplace_back(mod.path());
    }
}

}  // namespace of
