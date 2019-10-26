#pragma once

#include <Eigen/Core>

namespace spide2d {

struct camera {
    [[nodiscard]] const Eigen::Matrix4f &projection_matrix() const noexcept;

    void set_viewport(Eigen::Vector2d min, Eigen::Vector2d max) noexcept;

private:
    Eigen::Matrix4f projection_matrix_;
};

}  // namespace spide2d
