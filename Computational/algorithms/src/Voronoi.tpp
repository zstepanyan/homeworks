#ifndef VORONOI_TPP
#define VORONOI_TPP

#include "../include/Voronoi.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

template <typename T>
Voronoi<T>::Voronoi(const std::vector<point_t>& sites) {
    compute(sites);
}

template <typename T>
Voronoi<T>::Voronoi(const Delaunay<T>& delaunay) {
    compute(delaunay);
}

template <typename T>
void Voronoi<T>::compute(const std::vector<point_t>& sites) {
    clear();
    delaunay_.triangulate(sites);
    build_from_delaunay();
}

template <typename T>
void Voronoi<T>::compute(const Delaunay<T>& delaunay) {
    clear();
    delaunay_ = delaunay;
    build_from_delaunay();
}

template <typename T>
void Voronoi<T>::build_from_delaunay() {
    const auto& triangles = delaunay_.triangles();
    const auto& points = delaunay_.points();

    if (triangles.empty() || points.empty()) {
        return;
    }

    // Compute circumcenters (Voronoi vertices)
    vertices_.clear();
    for (const auto& tri : triangles) {
        vertices_.push_back(tri.circumcenter());
    }

    // Create a cell for each site
    cells_.resize(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        cells_[i].site = points[i];
        cells_[i].vertices.clear();
        cells_[i].neighbor_cells.clear();
        cells_[i].is_bounded = true;
        site_to_cell_[i] = i;
    }

    // Build adjacency: for each site, find all triangles containing it
    // The circumcenters of these triangles form the Voronoi cell vertices
    for (std::size_t site_idx = 0; site_idx < points.size(); ++site_idx) {
        compute_cell_vertices(site_idx);
    }

    // Identify unbounded cells (sites on convex hull)
    identify_unbounded_cells();

    // Build Voronoi edges
    edges_.clear();
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        const auto& tri = triangles[i];
        point_t cc_i = vertices_[i];

        // For each edge of the triangle
        for (int e = 0; e < 3; ++e) {
            point_t p1 = tri.vertices[e];
            point_t p2 = tri.vertices[(e + 1) % 3];

            // Find adjacent triangle sharing this edge
            bool found_neighbor = false;
            for (std::size_t j = i + 1; j < triangles.size(); ++j) {
                const auto& other = triangles[j];
                bool has_p1 = false, has_p2 = false;

                for (int k = 0; k < 3; ++k) {
                    if (other.vertices[k] == p1) has_p1 = true;
                    if (other.vertices[k] == p2) has_p2 = true;
                }

                if (has_p1 && has_p2) {
                    // Adjacent triangles - connect their circumcenters
                    edges_.emplace_back(cc_i, vertices_[j]);
                    found_neighbor = true;
                    break;
                }
            }

            // If no neighbor found, this is a hull edge -> infinite Voronoi edge
            if (!found_neighbor) {
                // Create perpendicular bisector direction pointing outward
                auto dx = static_cast<T>(p2[0]) - static_cast<T>(p1[0]);
                auto dy = static_cast<T>(p2[1]) - static_cast<T>(p1[1]);

                // Perpendicular direction
                point_t perp{-dy, dx};

                // Determine outward direction (away from third vertex)
                auto third = tri.vertices[(e + 2) % 3];
                point_t mid{(static_cast<T>(p1[0]) + static_cast<T>(p2[0])) / 2,
                            (static_cast<T>(p1[1]) + static_cast<T>(p2[1])) / 2};

                auto to_third_x = static_cast<T>(third[0]) - static_cast<T>(mid[0]);
                auto to_third_y = static_cast<T>(third[1]) - static_cast<T>(mid[1]);

                auto dot = static_cast<T>(perp[0]) * to_third_x + static_cast<T>(perp[1]) * to_third_y;
                if (dot > 0) {
                    perp = point_t{dy, -dx};  // Flip direction
                }

                edges_.push_back(edge_t::infinite(cc_i, perp));
            }
        }
    }

    // Order vertices CCW for each cell
    for (auto& cell : cells_) {
        order_cell_vertices_ccw(cell);
    }
}

template <typename T>
void Voronoi<T>::compute_cell_vertices(std::size_t site_idx) {
    const auto& triangles = delaunay_.triangles();
    const auto& points = delaunay_.points();

    if (site_idx >= points.size()) {
        return;
    }

    Cell& cell = cells_[site_idx];
    const point_t& site = points[site_idx];

    // Find all triangles containing this site
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        const auto& tri = triangles[i];
        bool contains_site = false;

        for (int j = 0; j < 3; ++j) {
            if (tri.vertices[j] == site) {
                contains_site = true;
                break;
            }
        }

        if (contains_site) {
            cell.vertices.push_back(vertices_[i]);
        }
    }
}

template <typename T>
void Voronoi<T>::order_cell_vertices_ccw(Cell& cell) {
    if (cell.vertices.size() < 3) {
        return;
    }

    // Compute centroid
    auto cx = static_cast<T>(0);
    auto cy = static_cast<T>(0);
    for (const auto& v : cell.vertices) {
        cx += static_cast<T>(v[0]);
        cy += static_cast<T>(v[1]);
    }
    cx /= static_cast<T>(cell.vertices.size());
    cy /= static_cast<T>(cell.vertices.size());

    // Sort by angle from centroid
    std::sort(cell.vertices.begin(), cell.vertices.end(),
        [cx, cy](const point_t& a, const point_t& b) {
            double angle_a = std::atan2(static_cast<double>(a[1]) - cy,
                                        static_cast<double>(a[0]) - cx);
            double angle_b = std::atan2(static_cast<double>(b[1]) - cy,
                                        static_cast<double>(b[0]) - cx);
            return angle_a < angle_b;
        });
}

template <typename T>
void Voronoi<T>::identify_unbounded_cells() {
    // Sites on the convex hull have unbounded Voronoi cells
    std::vector<point_t> hull = delaunay_.convex_hull();
    const auto& points = delaunay_.points();

    for (std::size_t i = 0; i < points.size(); ++i) {
        for (const auto& hp : hull) {
            if (points[i] == hp) {
                cells_[i].is_bounded = false;
                break;
            }
        }
    }
}

template <typename T>
const std::vector<typename Voronoi<T>::Cell>& Voronoi<T>::cells() const {
    return cells_;
}

template <typename T>
const std::vector<typename Voronoi<T>::edge_t>& Voronoi<T>::edges() const {
    return edges_;
}

template <typename T>
const std::vector<typename Voronoi<T>::point_t>& Voronoi<T>::vertices() const {
    return vertices_;
}

template <typename T>
std::size_t Voronoi<T>::cell_count() const {
    return cells_.size();
}

template <typename T>
std::size_t Voronoi<T>::locate(const point_t& p) const {
    // Find cell containing point (nearest site)
    auto min_dist_sq = std::numeric_limits<T>::max();
    std::size_t nearest = 0;

    for (std::size_t i = 0; i < cells_.size(); ++i) {
        auto dx = static_cast<T>(p[0]) - static_cast<T>(cells_[i].site[0]);
        auto dy = static_cast<T>(p[1]) - static_cast<T>(cells_[i].site[1]);
        auto dist_sq = dx * dx + dy * dy;

        if (dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            nearest = i;
        }
    }

    return nearest;
}

template <typename T>
const typename Voronoi<T>::Cell* Voronoi<T>::cell_for_site(const point_t& site) const {
    for (const auto& cell : cells_) {
        if (cell.site == site) {
            return &cell;
        }
    }
    return nullptr;
}

template <typename T>
std::vector<typename Voronoi<T>::point_t> Voronoi<T>::nearest_neighbors(
    const point_t& p, std::size_t k) const {

    std::vector<std::pair<T, point_t>> distances;

    for (const auto& cell : cells_) {
        auto dx = static_cast<T>(p[0]) - static_cast<T>(cell.site[0]);
        auto dy = static_cast<T>(p[1]) - static_cast<T>(cell.site[1]);
        auto dist_sq = dx * dx + dy * dy;
        distances.emplace_back(dist_sq, cell.site);
    }

    std::sort(distances.begin(), distances.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    std::vector<point_t> result;
    for (std::size_t i = 0; i < std::min(k, distances.size()); ++i) {
        result.push_back(distances[i].second);
    }

    return result;
}

template <typename T>
void Voronoi<T>::clip_to_bounds(const BoundingBox<T, 2>& bounds) {
    bounds_ = bounds;
    is_clipped_ = true;

    for (auto& cell : cells_) {
        clip_cell(cell, bounds);
    }

    // Update edges - replace infinite edges with clipped versions
    std::vector<edge_t> clipped_edges;
    for (const auto& e : edges_) {
        if (e.is_infinite) {
            point_t end = clip_infinite_edge(e, bounds);
            clipped_edges.emplace_back(e.p1_, end);
        } else {
            clipped_edges.push_back(e);
        }
    }
    edges_ = std::move(clipped_edges);
}

template <typename T>
void Voronoi<T>::clip_to_bounds(T margin) {
    if (cells_.empty()) {
        return;
    }

    // Compute bounds from sites
    BoundingBox<T, 2> bounds;
    for (const auto& cell : cells_) {
        bounds.expand(cell.site);
    }

    // Add margin
    auto min_pt = bounds.min();
    auto max_pt = bounds.max();

    auto size_x = static_cast<T>(max_pt[0]) - static_cast<T>(min_pt[0]);
    auto size_y = static_cast<T>(max_pt[1]) - static_cast<T>(min_pt[1]);
    auto expand = std::max(size_x, size_y) * margin + static_cast<T>(1);

    BoundingBox<T, 2> expanded_bounds;
    expanded_bounds.expand(point_t{static_cast<T>(min_pt[0]) - expand,
                                   static_cast<T>(min_pt[1]) - expand});
    expanded_bounds.expand(point_t{static_cast<T>(max_pt[0]) + expand,
                                   static_cast<T>(max_pt[1]) + expand});

    clip_to_bounds(expanded_bounds);
}

template <typename T>
typename Voronoi<T>::point_t Voronoi<T>::clip_infinite_edge(
    const edge_t& e, const BoundingBox<T, 2>& bounds) const {

    auto origin = e.p1_;
    auto dir = e.direction_;

    // Normalize direction
    auto len = std::sqrt(static_cast<double>(dir[0]) * static_cast<double>(dir[0]) +
                           static_cast<double>(dir[1]) * static_cast<double>(dir[1]));
    if (len < TOLERANCE) {
        return origin;
    }

    auto dx = static_cast<T>(static_cast<double>(dir[0]) / len);
    auto dy = static_cast<T>(static_cast<double>(dir[1]) / len);

    // Find intersection with bounding box
    auto min_pt = bounds.min();
    auto max_pt = bounds.max();

    auto t_max = std::numeric_limits<T>::max();

    // Check each boundary
    if (std::abs(static_cast<double>(dx)) > TOLERANCE) {
        auto t1 = (static_cast<T>(min_pt[0]) - static_cast<T>(origin[0])) / dx;
        auto t2 = (static_cast<T>(max_pt[0]) - static_cast<T>(origin[0])) / dx;
        if (t1 > 0) t_max = std::min(t_max, t1);
        if (t2 > 0) t_max = std::min(t_max, t2);
    }

    if (std::abs(static_cast<double>(dy)) > TOLERANCE) {
        auto t1 = (static_cast<T>(min_pt[1]) - static_cast<T>(origin[1])) / dy;
        auto t2 = (static_cast<T>(max_pt[1]) - static_cast<T>(origin[1])) / dy;
        if (t1 > 0) t_max = std::min(t_max, t1);
        if (t2 > 0) t_max = std::min(t_max, t2);
    }

    // Limit t_max to a reasonable value
    auto bound_size = std::max(static_cast<T>(max_pt[0]) - static_cast<T>(min_pt[0]),
                            static_cast<T>(max_pt[1]) - static_cast<T>(min_pt[1]));
    t_max = std::min(t_max, bound_size * 10);

    return point_t{static_cast<T>(origin[0]) + dx * t_max,
                   static_cast<T>(origin[1]) + dy * t_max};
}

template <typename T>
void Voronoi<T>::clip_cell(Cell& cell, const BoundingBox<T, 2>& bounds) {
    if (cell.vertices.empty()) {
        return;
    }

    // Sutherland-Hodgman clipping algorithm
    auto min_pt = bounds.min();
    auto max_pt = bounds.max();

    auto clip_polygon_to_edge = [](std::vector<point_t>& polygon,
                                   const point_t& edge_start,
                                   const point_t& edge_end) {
        if (polygon.empty()) return;

        std::vector<point_t> output;
        auto ex = static_cast<T>(edge_end[0]) - static_cast<T>(edge_start[0]);
        auto ey = static_cast<T>(edge_end[1]) - static_cast<T>(edge_start[1]);

        auto inside = [&](const point_t& p) {
            auto px = static_cast<T>(p[0]) - static_cast<T>(edge_start[0]);
            auto py = static_cast<T>(p[1]) - static_cast<T>(edge_start[1]);
            return ex * py - ey * px >= 0;
        };

        auto intersect_line = [&](const point_t& p1, const point_t& p2) {
            auto x1 = static_cast<T>(p1[0]), y1 = static_cast<T>(p1[1]);
            auto x2 = static_cast<T>(p2[0]), y2 = static_cast<T>(p2[1]);
            auto x3 = static_cast<T>(edge_start[0]), y3 = static_cast<T>(edge_start[1]);
            auto x4 = static_cast<T>(edge_end[0]), y4 = static_cast<T>(edge_end[1]);

            auto denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (std::abs(static_cast<double>(denom)) < 1e-9) {
                return p1;  // Parallel lines
            }

            auto t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
            return point_t{x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
        };

        for (std::size_t i = 0; i < polygon.size(); ++i) {
            const point_t& current = polygon[i];
            const point_t& next = polygon[(i + 1) % polygon.size()];

            if (inside(current)) {
                if (inside(next)) {
                    output.push_back(next);
                } else {
                    output.push_back(intersect_line(current, next));
                }
            } else if (inside(next)) {
                output.push_back(intersect_line(current, next));
                output.push_back(next);
            }
        }

        polygon = std::move(output);
    };

    // Clip against all four edges
    point_t bl{static_cast<T>(min_pt[0]), static_cast<T>(min_pt[1])};
    point_t br{static_cast<T>(max_pt[0]), static_cast<T>(min_pt[1])};
    point_t tr{static_cast<T>(max_pt[0]), static_cast<T>(max_pt[1])};
    point_t tl{static_cast<T>(min_pt[0]), static_cast<T>(max_pt[1])};

    clip_polygon_to_edge(cell.vertices, bl, br);  // Bottom
    clip_polygon_to_edge(cell.vertices, br, tr);  // Right
    clip_polygon_to_edge(cell.vertices, tr, tl);  // Top
    clip_polygon_to_edge(cell.vertices, tl, bl);  // Left
}

template <typename T>
std::vector<Polygon<T, 2>> Voronoi<T>::cell_polygons() const {
    std::vector<Polygon<T, 2>> result;

    for (const auto& cell : cells_) {
        if (cell.is_bounded || is_clipped_) {
            Polygon<T, 2> poly;
            for (const auto& v : cell.vertices) {
                poly.push_back(v);
            }
            result.push_back(std::move(poly));
        }
    }

    return result;
}

template <typename T>
const Delaunay<T>& Voronoi<T>::delaunay() const {
    return delaunay_;
}

template <typename T>
void Voronoi<T>::clear() {
    delaunay_.clear();
    cells_.clear();
    edges_.clear();
    vertices_.clear();
    site_to_cell_.clear();
    is_clipped_ = false;
}

#endif // VORONOI_TPP
