#pragma once

#include "mod.hpp"

#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spide2d/window.hpp>

namespace of {

/**
 * @brief This class contains all top-level elements that are specific to a new or restored game.
 *
 */
struct game {
public:
    game(std::unique_ptr<spide2d::window> window);

private:
    void setup_logging();
    void load_mods();

    std::shared_ptr<spdlog::logger>  logger_ {spdlog::stdout_color_mt("game")};
    std::unique_ptr<spide2d::window> window_;
    std::vector<mod>                 mods_;
};

}  // namespace of