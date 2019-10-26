#include "context.hpp"

#include <exception>
#include <lua.hpp>
#include <map>

namespace of::script {

std::map<lua_State *, context *> lua_back_ref;

struct script_error : std::runtime_error {
    script_error(lua_State *L) : std::runtime_error(fmt::format("Lua-script error: \n  | {}", lua_tostring(L, -1))) {}
};

static int l_my_print(lua_State *L) {
    lua_back_ref.at(L)->lua_log_callback();
    return 0;
}

static const struct luaL_Reg printlib[] = {
    {"print", l_my_print}, {NULL, NULL} /* end of array */
};

context::context(std::shared_ptr<spdlog::logger> logger) noexcept : logger_(std::move(logger)) {
    L = luaL_newstate();
    if (!L) {
        spdlog::critical("Cannot create lua state.");
        std::terminate();
    }
    lua_back_ref[L] = this;
    luaL_openlibs(L);
    lua_getglobal(L, "_G");
    luaL_setfuncs(L, printlib, 0);
    lua_pop(L, 1);
}

context::~context() {
    lua_back_ref.erase(L);
    lua_close(L);
}

void context::load(const fs::path &lua_file) {
    if (luaL_loadfile(L, lua_file.string().c_str()))
        throw script_error(L);

    if (lua_pcall(L, 0, 0, 0))
        throw script_error(L);
}

void context::lua_log_callback() {
    const auto nargs = lua_gettop(L);
    for (int i = 1; i <= nargs; ++i) {
        logger_->info("{}", lua_tostring(L, i));
    }
}

}  // namespace of::script