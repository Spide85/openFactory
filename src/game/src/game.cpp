#include "game.hpp"

#include "ressources.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace of {

game::game(std::unique_ptr<spide2d::window> window) : window_(std::move(window)) {
    setup_logging();
    load_mods();
}

void game::setup_logging() {
    auto console = spdlog::stdout_color_mt("game_logger");
    console->info("Logger created.");
}

void game::load_mods() {
    for (const auto mod : fs::directory_iterator(mods_dir())) {
        spdlog::info(mod.path().string());
    }
}

}  // namespace of
