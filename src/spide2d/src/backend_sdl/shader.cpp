/*#include "util.hpp"

#include <array>
#include <glad/glad.h>
#include <iostream>
#include <spide2d/shader.hpp>

namespace spide {

template<>
void shader::set_var<bool>(const std::string &name, const bool &value) {
    glUniform1i(glGetUniformLocation(gl_program_id, name.c_str()), static_cast<int>(value));
}
template<>
void shader::set_var(const std::string &name, const int &value) {
    glUniform1i(glGetUniformLocation(gl_program_id, name.c_str()), value);
}
template<>
void shader::set_var(const std::string &name, const unsigned int &value) {
    glUniform1i(glGetUniformLocation(gl_program_id, name.c_str()), value);
}
template<>
void shader::set_var(const std::string &name, const float &value) {
    glUniform1f(glGetUniformLocation(gl_program_id, name.c_str()), value);
}
template<>
void shader::set_var(const std::string &name, const Eigen::Vector2f &value) {
    glUniform2fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, &value[0]);
}
template<>
void shader::set_var(const std::string &name, const Eigen::Vector3f &value) {
    glUniform3fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, &value[0]);
}
template<>
void shader::set_var(const std::string &name, const Eigen::Vector4f &value) {
    glUniform4fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, &value[0]);
}
template<>
void shader::set_var(const std::string &name, const Eigen::Matrix2f &value) {
    glUniformMatrix2fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, GL_FALSE, &value(0, 0));
}
template<>
void shader::set_var(const std::string &name, const Eigen::Matrix3f &value) {
    glUniformMatrix3fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, GL_FALSE, &value(0, 0));
}
template<>
void shader::set_var(const std::string &name, const Eigen::Matrix4f &value) {
    glUniformMatrix4fv(glGetUniformLocation(gl_program_id, name.c_str()), 1, GL_FALSE, &value(0, 0));
}

shader::~shader() {
    glDeleteProgram(gl_program_id);
}

std::unique_ptr<shader> shader::create(const std::vector<program_stage> &stages) {
    auto new_shader = std::unique_ptr<shader>(new shader());
    for (const auto &stage : stages) {
        new_shader->add_program_stage(stage);
    }
    new_shader->link_stages();
    return new_shader;
}

std::unique_ptr<shader> shader::create(const std::string &vertex_shader_source,
                                       const std::string &fragment_shader_source) {
    std::vector<program_stage> stages;
    stages.push_back({vertex_shader_source, program_stage_type::VERTEX});
    stages.push_back({fragment_shader_source, program_stage_type::FRAGMENT});
    return create(stages);
}

std::unique_ptr<shader> shader::create(const std::string &shader_file_basename) {
    std::map<std::string, program_stage_type> shader_string_types = {
        {"vertex", program_stage_type::VERTEX},
        {"fragment", program_stage_type::FRAGMENT},
        {"tess_con", program_stage_type::TESSELLATION_CONTROL},
        {"tess_eval", program_stage_type::TESSELLATION_EVALUATION},
        {"geometry", program_stage_type::GEOMETRY}};

    std::vector<program_stage> stages;

    for (const auto &shader_string_type : shader_string_types) {
        try {
            auto shader_source = util::read_whole_file(shader_file_basename + "." + shader_string_type.first + ".glsl");
            stages.push_back({shader_source, shader_string_type.second});
        } catch (...) {
        }
    }

    return create(stages);
}

void shader::use() {
    glUseProgram(gl_program_id);
}

void shader::add_program_stage(const program_stage &stage_to_add) {
    GLenum gl_shader_type;
    GLint  MaxPatchVertices = 0;

    switch (stage_to_add.type) {
    case program_stage_type::VERTEX: gl_shader_type = GL_VERTEX_SHADER; break;
    case program_stage_type::FRAGMENT: gl_shader_type = GL_FRAGMENT_SHADER; break;
    case program_stage_type::GEOMETRY: gl_shader_type = GL_GEOMETRY_SHADER; break;
    case program_stage_type::TESSELLATION_CONTROL: gl_shader_type = GL_TESS_CONTROL_SHADER; break;
    case program_stage_type::TESSELLATION_EVALUATION: gl_shader_type = GL_TESS_EVALUATION_SHADER; break;
    default: throw std::runtime_error("Shader type not supported.");
    }

    auto gl_shader_id = glCreateShader(gl_shader_type);

    const char *source_as_c_string = stage_to_add.program_source.c_str();
    glShaderSource(gl_shader_id, 1, &source_as_c_string, 0);
    glCompileShader(gl_shader_id);

    int success = GL_FALSE;
    glGetShaderiv(gl_shader_id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        std::array<char, 512> log_message;
        glGetShaderInfoLog(gl_shader_id, 512, 0, log_message.data());
        std::cerr << "Shader compilation failed: " << log_message.data() << std::endl;
        throw std::runtime_error("Shader compilation failed.");
    }

    program_stage_ids[stage_to_add.type] = gl_shader_id;
}

void shader::link_stages() {
    if (program_stage_ids.empty())
        throw std::runtime_error("Tried to link shader program with no shader stages attached.");

    gl_program_id = glCreateProgram();
    for (const auto &stage : program_stage_ids) {
        glAttachShader(gl_program_id, stage.second);
    }
    glLinkProgram(gl_program_id);

    int success = GL_FALSE;
    glGetProgramiv(gl_program_id, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        std::array<char, 512> log_message;
        glGetProgramInfoLog(gl_program_id, 4096, 0, log_message.data());
        std::cerr << "Shader linking failed: " << log_message.data() << std::endl;
        throw std::runtime_error("Shader linking failed.");
    }

    for (const auto &stage : program_stage_ids) {
        glDeleteShader(stage.second);
    }
    program_stage_ids.clear();
}
}  // namespace spide
*/