#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "../../core/include/Coordinate.hpp"
#include "../../core/include/Point.hpp"
#include <type_traits>
#include <algorithm>
#include <limits>
#include <cassert>

template <typename T, std::size_t N>
class BoundingBox {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
    static_assert(N > 0, "Dimension must be > 0");

public:
    using Point   = Point<T, N>;
    using coord_t = coord_t<T>;

    BoundingBox();
    BoundingBox(const BoundingBox& other);
    BoundingBox& operator=(const BoundingBox& other);
    BoundingBox(BoundingBox&& other) noexcept;
    BoundingBox& operator=(BoundingBox&& other) noexcept;

    void expand(const Point& p);
    void expand(const BoundingBox& other);
    void reset();

    const Point& min() const;
    const Point& max() const;

    Point center() const;
    Point size() const;
    bool empty() const;
    bool contains(const Point& p) const;
    bool intersects(const BoundingBox& other) const;

private:
    Point min_;
    Point max_;
};

template <typename T>
using BoundingBox2D = BoundingBox<T, 2>;

#include "../src/BoundingBox.tpp"

#endif