project('OpenFactory', ['cpp', 'c'], default_options : ['cpp_std=c++17'])

eigen = dependency('eigen')
spdlog = dependency('spdlog')
sdl2 = dependency('sdl2')
boost = dependency('boost')
json = subproject('json').get_variable('nlohmann_json_dep')


src_game = [
    'game/src/main.cpp',
    'game/src/game.cpp',
    'game/src/game.hpp',
]

##### GLAD #####
src_glad = [
    'glad/src/glad.c',
]

glad_includes = include_directories('glad/include')
glad_lib = static_library('glad', src_glad, include_directories: glad_includes)
glad_dependency = declare_dependency(include_directories: glad_includes, link_with: glad_lib)

cc = meson.get_compiler('c')
libdl = cc.find_library('dl')

##### Subdirs #####

subdir('spide2d')

##### Executable #####

executable(
    'openFactory', 
    [src_game],
    dependencies: [
        eigen, 
        spdlog,
        sdl2, 
        glad_dependency, 
        libdl,
        json,
        boost
    ], 
    include_directories: [glad_includes, spide2d_include_dirs],
    link_with: libspide2d,
    install: true, 
)