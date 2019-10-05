#pragma once

#include <stdexcept>
#include <string>

namespace spide2d {

struct load_texture_error : std::runtime_error {
    load_texture_error(const std::string &what) : std::runtime_error(what) {}
};

struct texture {
    texture(const std::string &filename);
    texture(const texture &&other);
    void operator=(const texture &&other);

    ~texture();

    void bind();

private:
    texture(const texture &) = delete;
    void operator=(const texture &) = delete;

    uint32_t gl_ref_;
};

}  // namespace spide2d
