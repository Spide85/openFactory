#pragma once

#include <glad/glad.h>
#include <Eigen/Dense>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace spide {

enum class program_stage_type { VERTEX, FRAGMENT, GEOMETRY, TESSELLATION_CONTROL, TESSELLATION_EVALUATION };

struct program_stage {
    std::string program_source;
    program_stage_type type;
};

struct shader {
    virtual ~shader();

    static std::unique_ptr<shader> create(const std::vector<program_stage> &stages);
    static std::unique_ptr<shader> create(const std::string &vertex_shader_source, const std::string &fragment_shader_source);
    static std::unique_ptr<shader> create(const std::string &shader_file_basename);

    void use();

    template <typename T>
    void set_var(const std::string &name, const T &value);

private:
    shader() = default;
    shader(shader &) = default;

    unsigned int gl_program_id;

    std::map<program_stage_type, unsigned int> program_stage_ids;

    void add_program_stage(const program_stage &stage_to_add);
    void link_stages();
};

}  // namespace spide
