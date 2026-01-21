#ifndef LINE_HPP
#define LINE_HPP

#include "../../core/include/Point.hpp"

template <typename T, std::size_t N>
struct Line {
    using point_t = Point<T, N>;

    point_t p1_;
    point_t p2_;

    Line(const point_t& point1, const point_t& point2) : p1_(point1), p2_(point2) {}
    Line(const Line&) = default;
    Line& operator=(const Line&) = default;
    Line(Line&&) noexcept = default;
    Line& operator=(Line&&) noexcept = default;
};

#endif // LINE_HPP
