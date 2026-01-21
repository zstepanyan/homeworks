#ifndef VORONOI_HPP
#define VORONOI_HPP

#include "../../core/include/Point.hpp"
#include "../../primitives/include/Edge.hpp"
#include "Delaunay.hpp"
#include "../../primitives/include/BoundingBox.hpp"
#include "../../primitives/include/Polygon.hpp"
#include <vector>
#include <unordered_map>

template <typename T>
class Voronoi {
public:
    using point_t = Point<T, 2>;
    using edge_t = Edge<T, 2>;
    static constexpr double TOLERANCE = 1e-9;

    struct Cell {
        point_t site;                           // Original input point (Delaunay vertex)
        std::vector<point_t> vertices;          // CCW ordered Voronoi vertices
        std::vector<std::size_t> neighbor_cells; // Adjacent cell indices
        bool is_bounded = true;                 // False if cell extends to infinity
    };

    // Construction
    Voronoi() = default;
    explicit Voronoi(const std::vector<point_t>& sites);
    explicit Voronoi(const Delaunay<T>& delaunay);

    Voronoi(const Voronoi&) = default;
    Voronoi& operator=(const Voronoi&) = default;
    Voronoi(Voronoi&&) noexcept = default;
    Voronoi& operator=(Voronoi&&) noexcept = default;

    // Build from points or existing triangulation
    void compute(const std::vector<point_t>& sites);
    void compute(const Delaunay<T>& delaunay);

    // Accessors
    const std::vector<Cell>& cells() const;
    const std::vector<edge_t>& edges() const;
    const std::vector<point_t>& vertices() const;
    std::size_t cell_count() const;

    // Queries
    std::size_t locate(const point_t& p) const;
    const Cell* cell_for_site(const point_t& site) const;
    std::vector<point_t> nearest_neighbors(const point_t& p, std::size_t k) const;

    // Bounded diagram (clip to bounding box)
    void clip_to_bounds(const BoundingBox<T, 2>& bounds);
    void clip_to_bounds(T margin = 0);

    // Export
    std::vector<Polygon<T, 2>> cell_polygons() const;
    const Delaunay<T>& delaunay() const;

    // Clear
    void clear();

private:
    Delaunay<T> delaunay_;
    std::vector<Cell> cells_;
    std::vector<edge_t> edges_;
    std::vector<point_t> vertices_;
    BoundingBox<T, 2> bounds_;
    bool is_clipped_ = false;

    // Construction helpers
    void build_from_delaunay();
    void compute_cell_vertices(std::size_t site_idx);
    void order_cell_vertices_ccw(Cell& cell);
    void identify_unbounded_cells();

    // Clipping helpers
    point_t clip_infinite_edge(const edge_t& e, const BoundingBox<T, 2>& bounds) const;
    void clip_cell(Cell& cell, const BoundingBox<T, 2>& bounds);

    // Map from site point to cell index
    std::unordered_map<std::size_t, std::size_t> site_to_cell_;
};

template <typename T>
using Voronoi2D = Voronoi<T>;

#include "../src/Voronoi.tpp"

#endif // VORONOI_HPP
