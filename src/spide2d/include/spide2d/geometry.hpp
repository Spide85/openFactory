#pragma once
#include <Eigen/Geometry>

namespace spide2d {

template<typename scalar, typename = typename std::enable_if<std::is_arithmetic<scalar>::value, scalar>::type>
struct rect {
    rect(scalar x, scalar y, scalar width, scalar height) noexcept {
        aabb_.max()[0] = x + width;
        aabb_.max()[1] = y + height;
        aabb_.min()[0] = std::move(x);
        aabb_.min()[1] = std::move(y);
    }

    rect() = default;

    [[nodiscard]] scalar x() const noexcept {
        return aabb_.min()[0];
    }
    [[nodiscard]] scalar y() const noexcept {
        return aabb_.min()[1];
    }
    [[nodiscard]] scalar width() const noexcept {
        return aabb_.max()[0] - aabb_.min()[0];
    }
    [[nodiscard]] scalar height() const noexcept {
        return aabb_.max()[1] - aabb_.min()[1];
    }
    [[nodiscard]] scalar &x() noexcept {
        return aabb_.min()[0];
    }
    [[nodiscard]] scalar &y() noexcept {
        return aabb_.min()[1];
    }
    [[nodiscard]] scalar &width() noexcept {
        return aabb_.max()[0] - aabb_.min()[0];
    }
    [[nodiscard]] scalar &height() noexcept {
        return aabb_.max()[1] - aabb_.min()[1];
    }
    void resize(scalar width, scalar height) noexcept {
        aabb_.max()[0] = aabb_.min()[0] + width;
        aabb_.max()[1] = aabb_.min()[1] + height;
    }
    void translate(scalar x, scalar y) noexcept {
        translate({std::move(x), std::move(y)});
    }
    void translate(Eigen::Matrix<scalar, 2, 1> new_size) noexcept {
        aabb_.translate(std::move(new_size));
    }

private:
    Eigen::AlignedBox<scalar, 2> aabb_;
};

}  // namespace spide2d
