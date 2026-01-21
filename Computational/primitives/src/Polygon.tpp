#ifndef POLYGON_TPP
#define POLYGON_TPP

#include <algorithm>
#include "../include/Polygon.hpp"

template <typename T, std::size_t N>
Polygon<T, N>::Polygon(const Polygon& other)
    : data_(other.data_), box_(other.box_), boxDirty_(other.boxDirty_) {}

template <typename T, std::size_t N>
Polygon<T, N>& Polygon<T, N>::operator=(const Polygon& other) {
    if (this != &other) {
        data_ = other.data_;
        box_ = other.box_;
        boxDirty_ = other.boxDirty_;
    }
    
    return *this;
}

template <typename T, std::size_t N>
Polygon<T, N>::Polygon(Polygon&& other) noexcept
    : data_(std::move(other.data_)),
      box_(std::move(other.box_)),
      boxDirty_(other.boxDirty_) {
    other.boxDirty_ = true;
}

template <typename T, std::size_t N>
Polygon<T, N>& Polygon<T, N>::operator=(Polygon&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
        box_ = std::move(other.box_);
        boxDirty_ = other.boxDirty_;
        other.boxDirty_ = true;
    }
    
    return *this;
}

template <typename T, std::size_t N>
BoundingBox<T, N> Polygon<T, N>::boundingBox() const {
    if (boxDirty_) {
        box_ = BoundingBox<T, N>();
        for (const auto& p : data_) box_.expand(p);
        boxDirty_ = false;
    }
    
    return box_;
}

template <typename T, std::size_t N>
typename Polygon<T, N>::Point& Polygon<T, N>::operator[](std::size_t ind) {
    return data_[ind];
}

template <typename T, std::size_t N>
const typename Polygon<T, N>::Point& Polygon<T, N>::operator[](std::size_t ind) const {
    return data_[ind];
}

template <typename T, std::size_t N>
typename Polygon<T, N>::Point& Polygon<T, N>::at(std::size_t ind) {
    boxDirty_ = true;
    return data_.at(ind);
}

template <typename T, std::size_t N>
const typename Polygon<T, N>::Point& Polygon<T, N>::at(std::size_t ind) const {
    return data_.at(ind);
}

template <typename T, std::size_t N>
bool Polygon<T, N>::empty() const { 
    return data_.empty(); 
}

template <typename T, std::size_t N>
std::size_t Polygon<T, N>::size() const { 
    return data_.size(); 
}

template <typename T, std::size_t N>
void Polygon<T, N>::clear() {
    data_.clear();
    boxDirty_ = true;
}

template <typename T, std::size_t N>
bool Polygon<T, N>::isConvex() const {
    const std::size_t n = data_.size();
    if (n < 3) {
        return true;
    }
    
    coord_t firstCross = coord_t{0};
    std::size_t start = 0;
    for (std::size_t i = 0; i < n; ++i) {
        Point v1 = data_[(i + 1) % n] - data_[i];
        Point v2 = data_[(i + 2) % n] - data_[(i + 1) % n];
        coord_t cr = v1.cross(v2);
        if (cr != coord_t{0}) {
            firstCross = cr;
            start = i;
            break;
        }
    }
    
    if (firstCross == coord_t{0}) {
        return true;
    }
    
    for (std::size_t i = 0; i < n; ++i) {
        Point v1 = data_[(start + i + 1) % n] - data_[(start + i) % n];
        Point v2 = data_[(start + i + 2) % n] - data_[(start + i + 1) % n];
        coord_t cr = v1.cross(v2);
        if (cr * firstCross < coord_t{0}) {
            return false;
        }
    }

    return true;
}

template <typename T, std::size_t N>
bool Polygon<T, N>::contains(const Point& p) const {
    if (data_.size() < 3) {
        return false;
    }

    bool inside = false;
    coord_t px = p[0], py = p[1];
    const coord_t eps = coord_t<T>::TOLERANCE;
    for (std::size_t i = 0, j = data_.size() - 1; i < data_.size(); j = i++) {
        coord_t ax = data_[i][0], ay = data_[i][1];
        coord_t bx = data_[j][0], by = data_[j][1];
        if ((ay > py) != (by > py)) {
            coord_t denom = by - ay;
            if ((denom >= coord_t{0} ? denom : -denom) < eps) {
                continue;
            }
            
            coord_t t = (py - ay) / denom;
            coord_t crossX = ax + t * (bx - ax);
            if (px < crossX + eps) {
                inside = !inside;
            }
        }
    }
    
    return inside;
}

template <typename T, std::size_t N>
auto Polygon<T, N>::area() const { // ? 
    const std::size_t n = data_.size();
    if (n < 3) {
        return coord_t{0};
    }
    
    coord_t sum = coord_t{0};
    for (std::size_t i = 0; i < n; ++i) {
        const Point& a = data_[i];
        const Point& b = data_[(i + 1) % n];
        sum += a[0] * b[1] - b[0] * a[1];
    }
    
    return (sum >= coord_t{0} ? sum : -sum) * coord_t{0.5};
}

template <typename T, std::size_t N>
auto Polygon<T, N>::perimeter() const {
    const std::size_t n = data_.size();
    if (n < 2) {
        return coord_t{0};
    }
    
    coord_t len = coord_t{0};
    for (std::size_t i = 0; i < n; ++i) {
        len += (data_[(i + 1) % n] - data_[i]).length();
    }
    
    return len;
}

template <typename T, std::size_t N>
bool Polygon<T, N>::isClockwise() const {
    const std::size_t n = data_.size();
    if (n < 3) {
        return false;
    }
    
    coord_t sum = coord_t{0};
    for (std::size_t i = 0; i < n; ++i) {
        const Point& a = data_[i];
        const Point& b = data_[(i + 1) % n];
        sum += a[0] * b[1] - b[0] * a[1];
    }
    
    return sum > coord_t{0};
}

template <typename T, std::size_t N>
void Polygon<T, N>::makeCounterClockwise() {
    if (isClockwise()) {
        std::reverse(data_.begin(), data_.end());
        boxDirty_ = true;
    }
}

template <typename T, std::size_t N>
bool Polygon<T, N>::isSimple() const { // no self intersection
    const std::size_t n = data_.size();
    if (n < 4) {
        return true;
    }
    
    for (std::size_t i = 0; i < n; ++i) {
        Segment s1(data_[i], data_[(i + 1) % n]);
        for (std::size_t j = i + 2; j < n; ++j) {
            if (i == 0 && j == n - 1) {
                continue;
            }
            if (j == (i + 1) % n) {
                continue;
            }
            
            Segment s2(data_[j], data_[(j + 1) % n]);
            if (s1.intersects(s2)) {
                return false;
            }
        }
    }

    return true;
}

template <typename T, std::size_t N>
std::vector<typename Polygon<T, N>::Segment> Polygon<T, N>::edges() const {
    std::vector<Segment> vec;
    const std::size_t n = data_.size();
    if (n < 2) {
        return vec;
    }
    
    vec.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        vec.emplace_back(data_[i], data_[(i + 1) % n]);
    }
    
    return vec;
}

template <typename T, std::size_t N>
std::vector<typename Polygon<T, N>::Point> Polygon<T, N>::vertices() const {
    return data_;
}

template <typename T, std::size_t N>
void Polygon<T, N>::push_back(const Point& p) {
    data_.push_back(p);
    boxDirty_ = true;
}

template <typename T, std::size_t N>
void Polygon<T, N>::insert(std::size_t index, const Point& p) {
    assert(index <= data_.size());
    data_.insert(data_.begin() + index, p);
    boxDirty_ = true;
}

#endif // POLYGON_TPP