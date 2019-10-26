#pragma once

#include <lua.hpp>

namespace of::script {
struct context {
    context() noexcept;
    ~context();

private:
    lua_State *lua_state_;
};
}  // namespace of::script
