#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "../../core/include/Point.hpp"
#include "Segment.hpp"
#include "BoundingBox.hpp"

#include <vector>
#include <cstddef>
#include <type_traits>
#include <cassert>

template <typename T, std::size_t N>
class Polygon {
    static_assert(N == 2, "Polygon is only defined for 2D (N == 2)");
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

public:
    using Point = Point<T, N>;
    using coord_t = coord_t<T>;
    using Segment = Segment<T, N>;

    Polygon() = default;
    Polygon(const Polygon& other);
    Polygon& operator=(const Polygon& other);
    Polygon(Polygon&& other) noexcept;
    Polygon& operator=(Polygon&& other) noexcept;
    ~Polygon() = default;

    BoundingBox<T, N> boundingBox() const;

    Point& operator[](std::size_t ind);
    const Point& operator[](std::size_t ind) const;
    Point& at(std::size_t ind);
    const Point& at(std::size_t ind) const;

    bool empty() const;
    std::size_t size() const;
    void clear();

    bool isConvex() const;
    bool contains(const Point& p) const;
    auto area() const;
    auto perimeter() const;
    bool isClockwise() const;
    void makeCounterClockwise();
    bool isSimple() const;

    std::vector<Segment> edges() const;
    std::vector<Point> vertices() const;

    void push_back(const Point& p);
    void insert(std::size_t index, const Point& p);

private:
    std::vector<Point> data_;
    mutable BoundingBox<T, N> box_;
    mutable bool boxDirty_ = true;
};

#include "../src/Polygon.tpp"

#endif