#pragma once

#include <filesystem>
#include <lua.hpp>
#include <spdlog/spdlog.h>

namespace of::script {

namespace fs = std::filesystem;

struct context {
    context(std::shared_ptr<spdlog::logger> logger) noexcept;
    ~context();

    void load(const fs::path &lua_file);

    void lua_log_callback();

private:
    lua_State *                     L;  // It seems common to name this L
    std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace of::script
