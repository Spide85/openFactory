project('OpenFactory', ['cpp', 'c'], default_options : ['cpp_std=c++17', 'warning_level=3', 'werror=true', 'default_library=static'])

# We first try system libs, if not found we pull it in by our own.
doctest_dep  = dependency('doctest', fallback: ['doctest',     'doctest_dep'])
entt_dep     = dependency('entt',    fallback: ['entt',        'entt_dep'   ])
eigen_dep    = dependency('eigen',   fallback: ['eigen',       'eigen_dep'  ])
spdlog_dep   = dependency('spdlog',  fallback: ['spdlog',      'spdlog_dep' ])
toml_dep     = dependency('toml',    fallback: ['toml',        'toml_dep'   ])
nuklear_dep  = dependency('nuklear', fallback: ['nuklear',     'nuklear_dep'])
lua_dep      = dependency('lua',     fallback: ['lua',         'lua_dep'    ])
sdl2_dep     = dependency('sdl2')   # Missing fallback
boost_dep    = dependency('boost')  # Missing fallback

# TODO

subproject('spdlog')

src_game = [
    'game/src/scripting/context.hpp',
    'game/src/scripting/context.cpp',
    'game/src/main.cpp',
    'game/src/game.cpp',
    'game/src/game.hpp',
    'game/src/mod.cpp',
    'game/src/mod.hpp',

    'game/tests/scripting.cpp',
]

src_spide2d = [
    'spide2d/src/backend_sdl/window.cpp',
    'spide2d/src/backend_sdl/window.hpp',
    'spide2d/src/backend_sdl/hotkey_registry.cpp',
    'spide2d/src/backend_sdl/hotkey_registry.hpp',
    'spide2d/src/backend_sdl/shader.cpp',
    'spide2d/src/backend_sdl/shader.hpp',
    'spide2d/src/backend_sdl/texture.cpp',
    'spide2d/src/backend_sdl/texture.hpp',
    'spide2d/src/backend_sdl/systems/ui_renderer.cpp',
    'spide2d/src/window.cpp',
    'spide2d/src/camera.cpp',
    'spide2d/src/components/sprite.cpp',
    
    'spide2d/tests/geometry.cpp',
]

spide2d_include_dirs = include_directories('spide2d/include')


##### GLAD #####
# TODO Rework
src_glad = [
    'extern/glad/src/glad.c',
]

glad_includes = include_directories('extern/glad/include')
glad_lib = static_library('glad', src_glad, include_directories: glad_includes)
glad_dep = declare_dependency(include_directories: glad_includes, link_with: glad_lib)

cc = meson.get_compiler('c')
libdl = cc.find_library('dl')


##### Executable #####

executable(
    'openFactory', 
    [src_game, src_spide2d],
    dependencies: [
        eigen_dep, 
        spdlog_dep,
        sdl2_dep, 
        glad_dep, 
        libdl,
        lua_dep,
        entt_dep,
        toml_dep,
        doctest_dep,
        nuklear_dep,
        boost_dep
    ], 
    include_directories: [glad_includes, spide2d_include_dirs],
    install: true, 
)

##### Install #####
install_subdir('extern/nuklear/extra_font', install_dir: '/usr/local/share/openFactory/')
