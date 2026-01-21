#ifndef TRIANGLE_TPP
#define TRIANGLE_TPP

#include <cmath>
#include <algorithm>
#include "../include/Triangle.hpp"

template <typename T, std::size_t N>
Triangle<T, N>::Triangle(const point_t& a, const point_t& b, const point_t& c) {
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
}

template <typename T, std::size_t N>
typename Triangle<T, N>::point_t Triangle<T, N>::circumcenter() const {
    const point_t& A = vertices[0];
    const point_t& B = vertices[1];
    const point_t& C = vertices[2];

    auto ax = A[0], ay = A[1];
    auto bx = B[0], by = B[1];
    auto cx = C[0], cy = C[1];

    auto D = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));

    if (std::abs(static_cast<double>(D)) < TOLERANCE) {
        // Degenerate triangle, return centroid
        return centroid();
    }

    auto ax2_ay2 = ax * ax + ay * ay;
    auto bx2_by2 = bx * bx + by * by;
    auto cx2_cy2 = cx * cx + cy * cy;

    auto ux = (ax2_ay2 * (by - cy) + bx2_by2 * (cy - ay) + cx2_cy2 * (ay - by)) / D;
    auto uy = (ax2_ay2 * (cx - bx) + bx2_by2 * (ax - cx) + cx2_cy2 * (bx - ax)) / D;

    return point_t{ux, uy};
}

template <typename T, std::size_t N>
T Triangle<T, N>::circumradius_squared() const {
    auto center = circumcenter();
    auto dx = static_cast<T>(vertices[0][0]) - static_cast<T>(center[0]);
    auto dy = static_cast<T>(vertices[0][1]) - static_cast<T>(center[1]);
    return dx * dx + dy * dy;
}

template <typename T, std::size_t N>
bool Triangle<T, N>::circumcircle_contains(const point_t& p) const {
    auto center = circumcenter();
    auto r_sq = circumradius_squared();

    auto dx = static_cast<T>(p[0]) - static_cast<T>(center[0]);
    auto dy = static_cast<T>(p[1]) - static_cast<T>(center[1]);
    auto dist_sq = dx * dx + dy * dy;

    return dist_sq < r_sq + static_cast<T>(TOLERANCE);
}

template <typename T, std::size_t N>
T Triangle<T, N>::area() const {
    const point_t& A = vertices[0];
    const point_t& B = vertices[1];
    const point_t& C = vertices[2];

    // Shoelace formula: 0.5 * ((Bx - Ax)(Cy - Ay) - (Cx - Ax)(By - Ay))
    auto cross = (static_cast<T>(B[0]) - static_cast<T>(A[0])) * (static_cast<T>(C[1]) - static_cast<T>(A[1])) -
              (static_cast<T>(C[0]) - static_cast<T>(A[0])) * (static_cast<T>(B[1]) - static_cast<T>(A[1]));
    return cross / 2;
}

template <typename T, std::size_t N>
T Triangle<T, N>::area_abs() const {
    auto a = area();
    return a < 0 ? -a : a;
}

template <typename T, std::size_t N>
typename Triangle<T, N>::point_t Triangle<T, N>::centroid() const {
    auto cx = (static_cast<T>(vertices[0][0]) + static_cast<T>(vertices[1][0]) + static_cast<T>(vertices[2][0])) / 3;
    auto cy = (static_cast<T>(vertices[0][1]) + static_cast<T>(vertices[1][1]) + static_cast<T>(vertices[2][1])) / 3;
    return point_t{cx, cy};
}

template <typename T, std::size_t N>
T Triangle<T, N>::perimeter() const {
    auto edge_length = [](const point_t& a, const point_t& b) {
        auto dx = static_cast<T>(b[0]) - static_cast<T>(a[0]);
        auto dy = static_cast<T>(b[1]) - static_cast<T>(a[1]);
        return std::sqrt(static_cast<double>(dx * dx + dy * dy));
    };

    return static_cast<T>(edge_length(vertices[0], vertices[1]) +
                          edge_length(vertices[1], vertices[2]) +
                          edge_length(vertices[2], vertices[0]));
}

template <typename T, std::size_t N>
typename Triangle<T, N>::point_t Triangle<T, N>::incenter() const {
    auto edge_length = [](const point_t& a, const point_t& b) {
        auto dx = static_cast<T>(b[0]) - static_cast<T>(a[0]);
        auto dy = static_cast<T>(b[1]) - static_cast<T>(a[1]);
        return std::sqrt(static_cast<double>(dx * dx + dy * dy));
    };

    auto a = edge_length(vertices[1], vertices[2]); // opposite to vertex 0
    auto b = edge_length(vertices[0], vertices[2]); // opposite to vertex 1
    auto c = edge_length(vertices[0], vertices[1]); // opposite to vertex 2

    auto p = a + b + c;
    if (p < TOLERANCE) {
        return centroid();
    }

    auto ix = static_cast<T>((a * static_cast<double>(vertices[0][0]) +
                           b * static_cast<double>(vertices[1][0]) +
                           c * static_cast<double>(vertices[2][0])) / p);
    auto iy = static_cast<T>((a * static_cast<double>(vertices[0][1]) +
                           b * static_cast<double>(vertices[1][1]) +
                           c * static_cast<double>(vertices[2][1])) / p);

    return point_t{ix, iy};
}

template <typename T, std::size_t N>
T Triangle<T, N>::inradius() const {
    auto a = area_abs();
    auto p = perimeter();
    if (p < static_cast<T>(TOLERANCE)) {
        return static_cast<T>(0);
    }
    return (2 * a) / p;
}

template <typename T, std::size_t N>
bool Triangle<T, N>::contains(const point_t& p) const {
    const point_t& A = vertices[0];
    const point_t& B = vertices[1];
    const point_t& C = vertices[2];

    auto sign = [](const point_t& p1, const point_t& p2, const point_t& p3) {
        return (static_cast<T>(p1[0]) - static_cast<T>(p3[0])) * (static_cast<T>(p2[1]) - static_cast<T>(p3[1])) -
               (static_cast<T>(p2[0]) - static_cast<T>(p3[0])) * (static_cast<T>(p1[1]) - static_cast<T>(p3[1]));
    };

    auto d1 = sign(p, A, B);
    auto d2 = sign(p, B, C);
    auto d3 = sign(p, C, A);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

template <typename T, std::size_t N>
bool Triangle<T, N>::is_ccw() const {
    return area() > 0;
}

template <typename T, std::size_t N>
bool Triangle<T, N>::is_degenerate() const {
    return std::abs(static_cast<double>(area())) < TOLERANCE;
}

template <typename T, std::size_t N>
Segment<T, N> Triangle<T, N>::edge(std::size_t i) const {
    return Segment<T, N>(vertices[i % 3], vertices[(i + 1) % 3]);
}

template <typename T, std::size_t N>
typename Triangle<T, N>::point_t Triangle<T, N>::opposite_vertex(std::size_t edge_idx) const {
    return vertices[(edge_idx + 2) % 3];
}

template <typename T, std::size_t N>
bool Triangle<T, N>::operator==(const Triangle& other) const {
    // Check if triangles have the same vertices (in any order)
    for (int i = 0; i < 3; ++i) {
        bool match = true;
        for (int j = 0; j < 3; ++j) {
            if (vertices[j] != other.vertices[(i + j) % 3]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

template <typename T, std::size_t N>
bool Triangle<T, N>::shares_edge(const Triangle& other) const {
    return shared_edge(other).has_value();
}

template <typename T, std::size_t N>
std::optional<Segment<T, N>> Triangle<T, N>::shared_edge(const Triangle& other) const {
    for (int i = 0; i < 3; ++i) {
        Segment<T, N> e1 = edge(i);
        for (int j = 0; j < 3; ++j) {
            Segment<T, N> e2 = other.edge(j);
            // Check both directions
            if ((e1.p1_ == e2.p1_ && e1.p2_ == e2.p2_) ||
                (e1.p1_ == e2.p2_ && e1.p2_ == e2.p1_)) {
                return e1;
            }
        }
    }
    return std::nullopt;
}

#endif // TRIANGLE_TPP
