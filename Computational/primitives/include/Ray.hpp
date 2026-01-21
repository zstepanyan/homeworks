#ifndef RAY_HPP
#define RAY_HPP

#include "../../core/include/Point.hpp"

template <typename T, std::size_t N>
struct Ray {
    using point_t = Point<T, N>;
    
    Ray(const point_t& point1, const point_t& point2) : p1_(point1), p2_(point2) {}
    Ray(const Ray&) = default;
    Ray& operator=(const Ray&) = default;
    Ray(Ray&&) noexcept = default;
    Ray& operator=(Ray&&) noexcept = default;

    point_t p1_; // start point
    point_t p2_; // point defining direction
};

#endif // RAY_HPP
