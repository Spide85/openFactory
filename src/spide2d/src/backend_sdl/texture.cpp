#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include <glad/glad.h>
#include <spdlog/fmt/fmt.h>

namespace spide2d {

texture::texture(const std::string &filename) {
    glGenTextures(1, &gl_ref_);
    glBindTexture(GL_TEXTURE_2D, gl_ref_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    auto *image_data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (!image_data)
        throw load_texture_error(fmt::format("Could not load {}.", filename));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image_data);
}

texture::texture(const texture &&other) {
    gl_ref_ = other.gl_ref_;
}

void texture::operator=(const texture &&other) {
    gl_ref_ = other.gl_ref_;
}

texture::~texture() {
    glDeleteTextures(1, &gl_ref_);
}

void texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_ref_);
}

}  // namespace spide2d
