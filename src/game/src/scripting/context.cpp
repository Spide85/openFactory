#include "context.hpp"

#include <exception>
#include <lua.hpp>
#include <spdlog/spdlog.h>

namespace of::script {
context::context() noexcept {
    lua_state_ = luaL_newstate();
    if (!lua_state_) {
        spdlog::critical("Cannot create lua state.");
        std::terminate();
    }
    luaL_openlibs(lua_state_);
}

context::~context() {
    lua_close(lua_state_);
}

}  // namespace of::script