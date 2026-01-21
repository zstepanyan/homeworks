#ifndef DELAUNAY_HPP
#define DELAUNAY_HPP

#include "../../core/include/Point.hpp"
#include "../../primitives/include/Triangle.hpp"
#include "../../primitives/include/Edge.hpp"
#include "../../primitives/include/BoundingBox.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class Delaunay {
public:
    using point_t = Point<T, 2>;
    using triangle_t = Triangle<T, 2>;
    using edge_t = Edge<T, 2>;
    static constexpr double TOLERANCE = 1e-9;

    Delaunay() = default;
    explicit Delaunay(const std::vector<point_t>& points);

    Delaunay(const Delaunay&) = default;
    Delaunay& operator=(const Delaunay&) = default;
    Delaunay(Delaunay&&) noexcept = default;
    Delaunay& operator=(Delaunay&&) noexcept = default;

    // Incremental construction
    void insert(const point_t& p);
    void insert(const std::vector<point_t>& points);
    void triangulate(const std::vector<point_t>& points);

    // Accessors
    const std::vector<triangle_t>& triangles() const;
    std::vector<edge_t> edges() const;
    const std::vector<point_t>& points() const;
    std::size_t triangle_count() const;
    std::size_t point_count() const;

    // Queries
    const triangle_t* locate(const point_t& p) const;
    std::vector<std::size_t> neighbors(std::size_t tri_idx) const;
    bool is_valid() const;

    // Convex hull
    std::vector<point_t> convex_hull() const;
    std::vector<edge_t> hull_edges() const;

    void clear();

private:
    std::vector<triangle_t> triangles_;
    std::vector<point_t> points_;
    triangle_t super_triangle_;
    bool has_super_triangle_ = false;
    point_t super_vertices_[3];

    // Bowyer-Watson helpers
    void create_super_triangle();
    void remove_super_triangle();
    std::vector<std::size_t> find_bad_triangles(const point_t& p) const;
    std::vector<edge_t> find_polygon_hole(const std::vector<std::size_t>& bad_indices) const;
    void remove_triangles(std::vector<std::size_t>& indices);

    // Helper to check if a point is a super-triangle vertex
    bool is_super_vertex(const point_t& p) const;
};

template <typename T>
using Delaunay2D = Delaunay<T>;

#include "../src/Delaunay.tpp"

#endif // DELAUNAY_HPP
