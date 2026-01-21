#ifndef DELAUNAY_TPP
#define DELAUNAY_TPP

#include "Delaunay.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

template <typename T>
Delaunay<T>::Delaunay(const std::vector<point_t>& points) {
    triangulate(points);
}

template <typename T>
void Delaunay<T>::triangulate(const std::vector<point_t>& points) {
    clear();
    if (points.size() < 3) {
        points_ = points;
        return;
    }

    points_ = points;
    create_super_triangle();
    for (const auto& p : points_) {
        insert(p);
    }

    remove_super_triangle();
}

template <typename T>
void Delaunay<T>::create_super_triangle() {
    if (points_.empty()) {
        return;
    }

    // Find bounding box
    auto min_x = std::numeric_limits<T>::max();
    auto min_y = std::numeric_limits<T>::max();
    auto max_x = std::numeric_limits<T>::lowest();
    auto max_y = std::numeric_limits<T>::lowest();
    for (const auto& p : points_) {
        auto px = static_cast<T>(p[0]);
        auto py = static_cast<T>(p[1]);
        min_x = std::min(min_x, px);
        min_y = std::min(min_y, py);
        max_x = std::max(max_x, px);
        max_y = std::max(max_y, py);
    }

    auto dx = max_x - min_x;
    auto dy = max_y - min_y;
    auto delta_max = std::max(dx, dy);
    auto mid_x = (min_x + max_x) / 2;
    auto mid_y = (min_y + max_y) / 2;

    // Create super-triangle that encompasses all points (3x the bounding box)
    super_vertices_[0] = point_t{mid_x - 20 * delta_max, mid_y - delta_max};
    super_vertices_[1] = point_t{mid_x, mid_y + 20 * delta_max};
    super_vertices_[2] = point_t{mid_x + 20 * delta_max, mid_y - delta_max};

    super_triangle_ = triangle_t(super_vertices_[0], super_vertices_[1], super_vertices_[2]);
    triangles_.push_back(super_triangle_);
    has_super_triangle_ = true;
}

template <typename T>
void Delaunay<T>::remove_super_triangle() {
    if (!has_super_triangle_) {
        return;
    }

    // Remove all triangles that share a vertex with the super-triangle
    triangles_.erase(
        std::remove_if(triangles_.begin(), triangles_.end(),
            [this](const triangle_t& tri) {
                for (int i = 0; i < 3; ++i) {
                    if (is_super_vertex(tri.vertices[i])) {
                        return true;
                    }
                }

                return false;
            }),

            triangles_.end()
        );

    has_super_triangle_ = false;
}

template <typename T>
bool Delaunay<T>::is_super_vertex(const point_t& p) const {
    for (int i = 0; i < 3; ++i) {
        if (p == super_vertices_[i]) {
            return true;
        }
    }

    return false;
}

template <typename T>
void Delaunay<T>::insert(const point_t& p) {
    // Find all triangles whose circumcircle contains the point
    std::vector<std::size_t> bad_triangles = find_bad_triangles(p);
    if (bad_triangles.empty()) {
        return;
    }

    // Find the polygon hole boundary
    std::vector<edge_t> polygon = find_polygon_hole(bad_triangles);

    // Remove bad triangles
    remove_triangles(bad_triangles);

    // Create new triangles from the point to each edge of the polygon
    for (const auto& edge : polygon) {
        triangles_.emplace_back(p, edge.p1_, edge.p2_);
    }
}

template <typename T>
void Delaunay<T>::insert(const std::vector<point_t>& points) {
    for (const auto& p : points) {
        // Add to points list if not already present
        bool found = false;
        for (const auto& existing : points_) {
            if (existing == p) {
                found = true;
                break;
            }
        }

        if (!found) {
            points_.push_back(p);
        }
    }

    // If we don't have a super-triangle yet, create one
    if (!has_super_triangle_ && !points_.empty()) {
        create_super_triangle();
    }

    for (const auto& p : points) {
        insert(p);
    }
}

template <typename T>
std::vector<std::size_t> Delaunay<T>::find_bad_triangles(const point_t& p) const {
    std::vector<std::size_t> bad;
    for (std::size_t i = 0; i < triangles_.size(); ++i) {
        if (triangles_[i].circumcircle_contains(p)) {
            bad.push_back(i);
        }
    }

    return bad;
}

template <typename T>
std::vector<typename Delaunay<T>::edge_t> Delaunay<T>::find_polygon_hole(
    const std::vector<std::size_t>& bad_indices) const {

    std::vector<edge_t> polygon;

    // Collect all edges from bad triangles
    std::vector<edge_t> all_edges;
    for (std::size_t idx : bad_indices) {
        const triangle_t& tri = triangles_[idx];
        all_edges.emplace_back(tri.vertices[0], tri.vertices[1]);
        all_edges.emplace_back(tri.vertices[1], tri.vertices[2]);
        all_edges.emplace_back(tri.vertices[2], tri.vertices[0]);
    }

    // Find edges that appear only once (boundary edges)
    for (std::size_t i = 0; i < all_edges.size(); ++i) {
        bool is_shared = false;
        for (std::size_t j = 0; j < all_edges.size(); ++j) {
            if (i != j && all_edges[i] == all_edges[j]) {
                is_shared = true;
                break;
            }
        }

        if (!is_shared) {
            polygon.push_back(all_edges[i]);
        }
    }

    return polygon;
}

template <typename T>
void Delaunay<T>::remove_triangles(std::vector<std::size_t>& indices) {
    // Sort indices in descending order to remove from back to front
    std::sort(indices.begin(), indices.end(), std::greater<std::size_t>());
    for (std::size_t idx : indices) {
        if (idx < triangles_.size()) {
            triangles_.erase(triangles_.begin() + idx);
        }
    }
}

template <typename T>
const std::vector<typename Delaunay<T>::triangle_t>& Delaunay<T>::triangles() const {
    return triangles_;
}

template <typename T>
std::vector<typename Delaunay<T>::edge_t> Delaunay<T>::edges() const {
    std::vector<edge_t> unique_edges;
    for (const auto& tri : triangles_) {
        for (int i = 0; i < 3; ++i) {
            edge_t e(tri.vertices[i], tri.vertices[(i + 1) % 3]);

            // Check if edge already exists
            bool found = false;
            for (const auto& existing : unique_edges) {
                if (existing == e) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                unique_edges.push_back(e);
            }
        }
    }

    return unique_edges;
}

template <typename T>
const std::vector<typename Delaunay<T>::point_t>& Delaunay<T>::points() const {
    return points_;
}

template <typename T>
std::size_t Delaunay<T>::triangle_count() const {
    return triangles_.size();
}

template <typename T>
std::size_t Delaunay<T>::point_count() const {
    return points_.size();
}

template <typename T>
const typename Delaunay<T>::triangle_t* Delaunay<T>::locate(const point_t& p) const {
    for (const auto& tri : triangles_) {
        if (tri.contains(p)) {
            return &tri;
        }
    }

    return nullptr;
}

template <typename T>
std::vector<std::size_t> Delaunay<T>::neighbors(std::size_t tri_idx) const {
    std::vector<std::size_t> result;
    if (tri_idx >= triangles_.size()) {
        return result;
    }

    const triangle_t& target = triangles_[tri_idx];
    for (std::size_t i = 0; i < triangles_.size(); ++i) {
        if (i == tri_idx) continue;

        if (target.shares_edge(triangles_[i])) {
            result.push_back(i);
        }
    }

    return result;
}

template <typename T>
bool Delaunay<T>::is_valid() const {
    // Verify Delaunay property: no point lies inside any triangle's circumcircle
    for (const auto& tri : triangles_) {
        for (const auto& p : points_) {
            // Skip vertices of the triangle
            bool is_vertex = false;
            for (int i = 0; i < 3; ++i) {
                if (p == tri.vertices[i]) {
                    is_vertex = true;
                    break;
                }
            }

            if (is_vertex) continue;

            // Check if point is strictly inside circumcircle
            auto center = tri.circumcenter();
            auto r_sq = tri.circumradius_squared();

            auto diff = p - center;
            auto dist_sq = diff.dot(diff);

            // If point is strictly inside (not just on boundary), triangulation is invalid
            if (dist_sq < r_sq - static_cast<T>(TOLERANCE)) {
                return false;
            }
        }
    }

    return true;
}

template <typename T>
std::vector<typename Delaunay<T>::point_t> Delaunay<T>::convex_hull() const {
    if (points_.size() < 3) {
        return points_;
    }

    // Andrew's monotone chain algorithm
    std::vector<point_t> sorted_points = points_;
    std::sort(sorted_points.begin(), sorted_points.end(),
        [](const point_t& a, const point_t& b) {
            if (static_cast<T>(a[0]) != static_cast<T>(b[0])) {
                return static_cast<T>(a[0]) < static_cast<T>(b[0]);
            }
    
            return static_cast<T>(a[1]) < static_cast<T>(b[1]);
        });

    auto cross = [](const point_t& O, const point_t& A, const point_t& B) {
        return (static_cast<T>(A[0]) - static_cast<T>(O[0])) * (static_cast<T>(B[1]) - static_cast<T>(O[1])) -
               (static_cast<T>(A[1]) - static_cast<T>(O[1])) * (static_cast<T>(B[0]) - static_cast<T>(O[0]));
    };

    std::vector<point_t> hull;

    // Build lower hull
    for (const auto& p : sorted_points) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], p) <= 0) {
            hull.pop_back();
        }
    
        hull.push_back(p);
    }

    // Build upper hull
    std::size_t lower_size = hull.size();
    for (auto it = sorted_points.rbegin(); it != sorted_points.rend(); ++it) {
        while (hull.size() > lower_size && cross(hull[hull.size()-2], hull[hull.size()-1], *it) <= 0) {
            hull.pop_back();
        }
    
        hull.push_back(*it);
    }

    hull.pop_back();  // Remove last point (same as first)
    return hull;
}

template <typename T>
std::vector<typename Delaunay<T>::edge_t> Delaunay<T>::hull_edges() const {
    std::vector<point_t> hull = convex_hull();
    std::vector<edge_t> result;
    if (hull.size() < 2) {
        return result;
    }

    for (std::size_t i = 0; i < hull.size(); ++i) {
        result.emplace_back(hull[i], hull[(i + 1) % hull.size()]);
    }

    return result;
}

template <typename T>
void Delaunay<T>::clear() {
    triangles_.clear();
    points_.clear();
    has_super_triangle_ = false;
}

#endif // DELAUNAY_TPP
