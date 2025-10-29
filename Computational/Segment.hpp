#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Point.hpp"

template <typename T, std::size_t N>
struct Segment {
    using point_t = Point<T, N>;

    point_t p1_;
    point_t p2_;

    Segment(const point_t& point1, const point_t& point2) : p1_(point1), p2_(point2) {}
    Segment(const Segment&) = default;
    Segment& operator=(const Segment&) = default;
    Segment(Segment&&) noexcept = default;
    Segment& operator=(Segment&&) noexcept = default;
};

#endif // SEGMENT_HPP

// class Ray; // Charagayt
// class Line; // ughigh
