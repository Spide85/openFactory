#pragma once
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <spdlog/fmt/fmt.h>
#include <string>

namespace of {

namespace fs = std::filesystem;

[[nodiscard]] const fs::path mods_dir() {
    const auto mods_path = fs::path(std::getenv("MODS_DIR"));
    if (!fs::exists(mods_path))
        throw std::runtime_error(
            fmt::format("Mods-Path does not exist. (MODS_DIR was set to: \"{}\")", mods_path.string()));
    return mods_path;
}

}  // namespace of
