#ifndef EDGE_HPP
#define EDGE_HPP

#include "../../core/include/Point.hpp"
#include "Line.hpp"
#include <cmath>
#include <functional>

template <typename T, std::size_t N = 2>
struct Edge {
    static_assert(N == 2, "Edge only supports 2D");

    using point_t = Point<T, N>;
    using vector_t = Vector<T, N>;
    static constexpr double TOLERANCE = 1e-9;

    point_t p1_;
    point_t p2_;
    bool is_infinite = false;
    point_t direction_;  // Direction for infinite edges

    // Constructors
    Edge() = default;
    Edge(const point_t& p1, const point_t& p2);
    static Edge infinite(const point_t& origin, const point_t& direction);

    Edge(const Edge&) = default;
    Edge& operator=(const Edge&) = default;
    Edge(Edge&&) noexcept = default;
    Edge& operator=(Edge&&) noexcept = default;

    // Properties
    T length_squared() const;
    double length() const;
    point_t midpoint() const;
    vector_t direction_vector() const;

    // Geometric operations
    Line<T, N> perpendicular_bisector() const;
    bool contains(const point_t& p) const;

    // Comparison (order-independent: {a,b} == {b,a})
    bool operator==(const Edge& other) const;
    bool operator!=(const Edge& other) const;
    std::size_t hash() const;
};

template <typename T>
using Edge2D = Edge<T, 2>;

// Hash function for use in unordered containers
template <typename T, std::size_t N>
struct EdgeHash {
    std::size_t operator()(const Edge<T, N>& e) const {
        return e.hash();
    }
};

#include "../src/Edge.tpp"

#endif // EDGE_HPP
