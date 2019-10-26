#include "mod.hpp"

#include <toml.hpp>

namespace of {

mod::mod(fs::path root_dir) : root_dir_(std::move(root_dir)) {
    parse_config();
    logger_ = spdlog::stdout_color_mt(name());
    logger_->info("Loading mod: {}", name());
    logger_->info("  | Created by: {}", author());
    logger_->info("  |       Mail: {}", author_mail());
    context_ = std::make_unique<script::context>(logger_);
    context_->load(script_root_ / "Main.lua");
}

const std::string &mod::name() noexcept {
    return name_;
}
const std::string &mod::author() noexcept {
    return author_;
}
const std::string &mod::author_mail() noexcept {
    return author_mail_;
}

void mod::parse_config() {
    const auto  toml_file = root_dir_ / "mod_info.toml";
    const auto  conf      = toml::parse(toml_file);
    const auto &general   = toml::find(conf, "general");
    name_                 = toml::find<std::string>(general, "name");
    author_               = toml::find<std::string>(general, "author");
    author_mail_          = toml::find<std::string>(general, "author_mail");
    const auto &script    = toml::find(conf, "script");
    script_root_          = root_dir_ / toml::find<std::string>(script, "root");
}

}  // namespace of
