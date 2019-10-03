
spide2d_include_dirs = include_directories('include')

source_files = [
    'src/backend_sdl/window.cpp',
    'src/backend_sdl/window.hpp',
    'src/backend_sdl/hotkey_registry.cpp',
    'src/backend_sdl/hotkey_registry.hpp',
    'src/window.cpp',
]

libspide2d = static_library('spide2d', 
    source_files, 
    dependencies: [
        eigen,
        spdlog,
        sdl2,
        glad_dependency,
        libdl,
        json,
        boost
    ], 
    include_directories: spide2d_include_dirs, 
    install: true
)

