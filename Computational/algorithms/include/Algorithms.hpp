#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "../../core/include/Point.hpp"
#include "../../primitives/include/Segment.hpp"
#include "../../primitives/include/Line.hpp"
#include "../../primitives/include/Ray.hpp"
#include "../../primitives/include/BoundingBox.hpp"
#include "../../primitives/include/Polygon.hpp"
#include <set>

// Forward declaration
template <typename T, std::size_t N>
struct Triangle;

namespace algo {

    // Orientation test: >0 CCW, <0 CW, =0 collinear
    template <typename T>
    T orientation(const Point<T, 2>& a, const Point<T, 2>& b, const Point<T, 2>& c) noexcept;

    // Closest point queries
    template <typename T, std::size_t N>
    Point<T, N> closest_point_on_segment(const Point<T, N>& p, const Segment<T, N>& seg) noexcept;

    template <typename T, std::size_t N>
    Point<T, N> closest_point_on_triangle(const Point<T, N>& p, const Triangle<T, N>& tri) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Segment<T,N>& seg1, const Segment<T,N>& seg2) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Ray<T,N>& ray1, const Ray<T,N>& ray2) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Line<T,N>& line1, const Line<T,N>& line2) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Line<T,N>& AB, const Segment<T,N>& CD) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Segment<T,N>& CD, const Line<T,N>& AB) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Ray<T,N>& AB, const Line<T,N>& CD) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Line<T,N>& CD, const Ray<T,N>& AB) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Segment<T,N>& AB, const Ray<T,N>& CD) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Ray<T,N>& CD, const Segment<T,N>& AB) noexcept;

    template <typename T, std::size_t N>
    constexpr bool intersect(const BoundingBox<T,N>& a, const BoundingBox<T,N>& b) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Polygon<T,N>& P1, const Polygon<T,N>& P2) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Polygon<T, N>& polygon, const Segment<T, N>& segment) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Segment<T, N>& segment, const Polygon<T, N>& polygon) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Polygon<T, N>& polygon, const Ray<T, N>& ray) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Ray<T, N>& ray, const Polygon<T, N>& polygon) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Polygon<T, N>& polygon, const Line<T, N>& line) noexcept;

    template <typename T, std::size_t N>
    bool intersect(const Line<T, N>& line, const Polygon<T, N>& polygon) noexcept;
    
} // namespace algo

#include "../src/Algorithms.tpp"

#endif // ALGORITHMS_HPP