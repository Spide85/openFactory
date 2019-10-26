#pragma once

#include "scripting/context.hpp"

#include <filesystem>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace of {

namespace fs = std::filesystem;

struct mod {
    mod(fs::path root_dir);

    [[nodiscard]] const std::string &name() noexcept;
    [[nodiscard]] const std::string &author() noexcept;
    [[nodiscard]] const std::string &author_mail() noexcept;

private:
    void parse_config();

    std::shared_ptr<spdlog::logger>  logger_;
    fs::path                         root_dir_;
    std::string                      name_;
    std::string                      author_;
    std::string                      author_mail_;
    fs::path                         script_root_;
    std::unique_ptr<script::context> context_;
};

}  // namespace of
