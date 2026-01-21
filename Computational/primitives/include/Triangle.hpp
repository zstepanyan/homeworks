#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "../../core/include/Point.hpp"
#include "Segment.hpp"
#include <cmath>
#include <optional>

template <typename T, std::size_t N = 2>
struct Triangle {
    static_assert(N == 2, "Triangle only supports 2D");

    using point_t = Point<T, N>;
    using coord_type = T;
    static constexpr double TOLERANCE = 1e-9;

    point_t vertices[3];

    Triangle() = default;
    Triangle(const point_t& a, const point_t& b, const point_t& c);
    Triangle(const Triangle&) = default;
    Triangle& operator=(const Triangle&) = default;
    Triangle(Triangle&&) noexcept = default;
    Triangle& operator=(Triangle&&) noexcept = default;

    point_t circumcenter() const;
    T circumradius_squared() const;
    bool circumcircle_contains(const point_t& p) const;

    // Basic properties
    T area() const;                         // Signed area (positive if CCW)
    T area_abs() const;                     // Absolute area
    point_t centroid() const;               // Center of mass
    T perimeter() const;

    // Incircle methods
    point_t incenter() const;               // Center of inscribed circle
    T inradius() const;                     // Radius of inscribed circle

    // Geometric tests
    bool contains(const point_t& p) const;  // Point inside triangle?
    bool is_ccw() const;                    // Counter-clockwise orientation?
    bool is_degenerate() const;             // Collinear vertices?

    // Edge access
    Segment<T, N> edge(std::size_t i) const;
    point_t opposite_vertex(std::size_t edge_idx) const;

    // Comparison
    bool operator==(const Triangle& other) const;
    bool shares_edge(const Triangle& other) const;
    std::optional<Segment<T, N>> shared_edge(const Triangle& other) const;
};

template <typename T>
using Triangle2D = Triangle<T, 2>;

#include "../src/Triangle.tpp"

#endif // TRIANGLE_HPP
