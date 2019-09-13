project('nlohmann_json', 'cpp', version : '3.4.0', license : 'MIT')

nlohmann_json_dep = declare_dependency(
    include_directories: include_directories('include')
)

install_subdir('include/nlohmann', install_dir: 'include')

pkgc = import('pkgconfig')
pkgc.generate(name: 'nlohmann_json',
    version: meson.project_version(),
    description: 'JSON for Modern C++'
)
