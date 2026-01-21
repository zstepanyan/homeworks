#ifndef BOUNDINGBOX_TPP
#define BOUNDINGBOX_TPP

#include "../include/BoundingBox.hpp"

template <typename T, std::size_t N>
BoundingBox<T, N>::BoundingBox() {
    reset();
}

template <typename T, std::size_t N>
BoundingBox<T, N>::BoundingBox(const BoundingBox& other)
    : min_(other.min_), max_(other.max_) {}

template <typename T, std::size_t N>
BoundingBox<T, N>& BoundingBox<T, N>::operator=(const BoundingBox& other) {
    if (this != &other) {
        min_ = other.min_;
        max_ = other.max_;
    }

    return *this;
}

template <typename T, std::size_t N>
BoundingBox<T, N>::BoundingBox(BoundingBox&& other) noexcept
    : min_(std::move(other.min_)), max_(std::move(other.max_)) {}

template <typename T, std::size_t N>
BoundingBox<T, N>& BoundingBox<T, N>::operator=(BoundingBox&& other) noexcept {
    if (this != &other) {
        min_ = std::move(other.min_);
        max_ = std::move(other.max_);
    }
    
    return *this;
}

template <typename T, std::size_t N>
void BoundingBox<T, N>::expand(const Point& p) {
    for (std::size_t i = 0; i < N; ++i) {
        min_[i] = std::min(min_[i], p[i]);
        max_[i] = std::max(max_[i], p[i]);
    }
}

template <typename T, std::size_t N>
void BoundingBox<T, N>::expand(const BoundingBox& other) {
    for (std::size_t i = 0; i < N; ++i) {
        min_[i] = std::min(min_[i], other.min_[i]);
        max_[i] = std::max(max_[i], other.max_[i]);
    }
}

template <typename T, std::size_t N>
void BoundingBox<T, N>::reset() {
    constexpr coord_t inf = std::numeric_limits<T>::has_infinity
        ? std::numeric_limits<T>::infinity()
        : std::numeric_limits<T>::max();

    constexpr coord_t ninf = std::numeric_limits<T>::has_infinity
        ? -std::numeric_limits<T>::infinity()
        : std::numeric_limits<T>::lowest();

    for (std::size_t i = 0; i < N; ++i) {
        min_[i] = inf;
        max_[i] = ninf;
    }
}

template <typename T, std::size_t N>
const typename BoundingBox<T, N>::Point& BoundingBox<T, N>::min() const {
    return min_;
}

template <typename T, std::size_t N>
const typename BoundingBox<T, N>::Point& BoundingBox<T, N>::max() const {
    return max_;
}

template <typename T, std::size_t N>
typename BoundingBox<T, N>::Point BoundingBox<T, N>::center() const {
    Point c;
    for (std::size_t i = 0; i < N; ++i) {
        c[i] = (min_[i] + max_[i]) * coord_t{0.5};
    }
    
    return c;
}

template <typename T, std::size_t N>
typename BoundingBox<T, N>::Point BoundingBox<T, N>::size() const {
    Point s;
    for (std::size_t i = 0; i < N; ++i) {
        s[i] = max_[i] - min_[i];
    }
    
    return s;
}

template <typename T, std::size_t N>
bool BoundingBox<T, N>::empty() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (min_[i] > max_[i]) {
            return true;
        }
    }

    return false;
}

template <typename T, std::size_t N>
bool BoundingBox<T, N>::contains(const Point& p) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (p[i] < min_[i] || p[i] > max_[i]) {
            return false;
        }
    }
    
    return true;
}

template <typename T, std::size_t N>
bool BoundingBox<T, N>::intersects(const BoundingBox& other) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (max_[i] < other.min_[i] || min_[i] > other.max_[i]) {
            return false;
        }
    }
    
    return true;
}

#endif