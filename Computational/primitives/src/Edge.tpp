#ifndef EDGE_TPP
#define EDGE_TPP

#include "../include/Edge.hpp"
#include <cmath>
#include <algorithm>

template <typename T, std::size_t N>
Edge<T, N>::Edge(const point_t& p1, const point_t& p2)
    : p1_(p1), p2_(p2), is_infinite(false), direction_() {}

template <typename T, std::size_t N>
Edge<T, N> Edge<T, N>::infinite(const point_t& origin, const point_t& direction) {
    Edge e;
    e.p1_ = origin;
    e.p2_ = origin;  // Not used for infinite edges
    e.is_infinite = true;
    e.direction_ = direction;
   
    return e;
}

template <typename T, std::size_t N>
T Edge<T, N>::length_squared() const {
    if (is_infinite) {
        return std::numeric_limits<T>::max();
    }
    
    auto dx = static_cast<T>(p2_[0]) - static_cast<T>(p1_[0]);
    auto dy = static_cast<T>(p2_[1]) - static_cast<T>(p1_[1]);

    return dx * dx + dy * dy;
}

template <typename T, std::size_t N>
double Edge<T, N>::length() const {
    if (is_infinite) {
        return std::numeric_limits<double>::infinity();
    }

    return std::sqrt(static_cast<double>(length_squared()));
}

template <typename T, std::size_t N>
typename Edge<T, N>::point_t Edge<T, N>::midpoint() const {
    auto mx = (static_cast<T>(p1_[0]) + static_cast<T>(p2_[0])) / 2;
    auto my = (static_cast<T>(p1_[1]) + static_cast<T>(p2_[1])) / 2;

    return point_t{mx, my};
}

template <typename T, std::size_t N>
typename Edge<T, N>::vector_t Edge<T, N>::direction_vector() const {
    if (is_infinite) {
        double len = std::sqrt(static_cast<double>(direction_[0]) * static_cast<double>(direction_[0]) +
                               static_cast<double>(direction_[1]) * static_cast<double>(direction_[1]));
        if (len < TOLERANCE) {
            return vector_t{static_cast<T>(1), static_cast<T>(0)};
        }
 
        return vector_t{static_cast<T>(static_cast<double>(direction_[0]) / len),
                        static_cast<T>(static_cast<double>(direction_[1]) / len)};
    }

    auto dx = static_cast<T>(p2_[0]) - static_cast<T>(p1_[0]);
    auto dy = static_cast<T>(p2_[1]) - static_cast<T>(p1_[1]);
    auto len = std::sqrt(static_cast<double>(dx * dx + dy * dy));
    if (len < TOLERANCE) {
        return vector_t{static_cast<T>(0), static_cast<T>(0)};
    }

    return vector_t{static_cast<T>(static_cast<double>(dx) / len),
                    static_cast<T>(static_cast<double>(dy) / len)};
}

template <typename T, std::size_t N>
Line<T, N> Edge<T, N>::perpendicular_bisector() const {
    auto mid = midpoint();

    // Direction of edge
    auto dx = static_cast<T>(p2_[0]) - static_cast<T>(p1_[0]);
    auto dy = static_cast<T>(p2_[1]) - static_cast<T>(p1_[1]);

    // Perpendicular direction (rotate 90 degrees)
    auto perp_x = -dy;
    auto perp_y = dx;

    // Second point on perpendicular bisector
    point_t p2{static_cast<T>(mid[0]) + perp_x, static_cast<T>(mid[1]) + perp_y};

    return Line<T, N>(mid, p2);
}

template <typename T, std::size_t N>
bool Edge<T, N>::contains(const point_t& p) const {
    if (is_infinite) {
        // Check if point is on the ray from p1_ in direction direction_
        auto dx = static_cast<T>(p[0]) - static_cast<T>(p1_[0]);
        auto dy = static_cast<T>(p[1]) - static_cast<T>(p1_[1]);

        // Cross product should be zero (collinear)
        auto cross = dx * static_cast<T>(direction_[1]) - dy * static_cast<T>(direction_[0]);
        if (std::abs(static_cast<double>(cross)) > TOLERANCE) {
            return false;
        }

        // Dot product should be non-negative (same direction)
        auto dot = dx * static_cast<T>(direction_[0]) + dy * static_cast<T>(direction_[1]);
        return dot >= 0;
    }

    // Check if point is on the segment
    auto dx = static_cast<T>(p2_[0]) - static_cast<T>(p1_[0]);
    auto dy = static_cast<T>(p2_[1]) - static_cast<T>(p1_[1]);
    auto px = static_cast<T>(p[0]) - static_cast<T>(p1_[0]);
    auto py = static_cast<T>(p[1]) - static_cast<T>(p1_[1]);

    // Cross product (should be zero for collinear points)
    auto cross = dx * py - dy * px;
    if (std::abs(static_cast<double>(cross)) > TOLERANCE) {
        return false;
    }

    // Dot product and length check
    auto dot = dx * px + dy * py;
    auto len_sq = dx * dx + dy * dy;

    return dot >= 0 && dot <= len_sq;
}

template <typename T, std::size_t N>
bool Edge<T, N>::operator==(const Edge& other) const {
    if (is_infinite != other.is_infinite) {
        return false;
    }

    if (is_infinite) {
        // For infinite edges, compare origin and direction
        return p1_ == other.p1_ && direction_ == other.direction_;
    }

    // Order-independent comparison for finite edges
    return (p1_ == other.p1_ && p2_ == other.p2_) ||
           (p1_ == other.p2_ && p2_ == other.p1_);
}

template <typename T, std::size_t N>
bool Edge<T, N>::operator!=(const Edge& other) const {
    return !(*this == other);
}

template <typename T, std::size_t N>
std::size_t Edge<T, N>::hash() const {
    // Order-independent hash
    auto point_hash = [](const point_t& p) {
        std::size_t h1 = std::hash<double>{}(static_cast<double>(p[0]));
        std::size_t h2 = std::hash<double>{}(static_cast<double>(p[1]));
        return h1 ^ (h2 << 1);
    };

    std::size_t h1 = point_hash(p1_);
    std::size_t h2 = point_hash(p2_);

    // XOR is commutative, so order doesn't matter
    return h1 ^ h2;
}

#endif // EDGE_TPP
