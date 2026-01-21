#ifndef COORDINATE_TPP
#define COORDINATE_TPP

#include "../include/Coordinate.hpp"
#include <cassert>

template <typename T>
coord_t<T>::coord_t() : value(T{}) {}

template <typename T>
coord_t<T>::coord_t(T val) : value(val) {}

template <typename T>
coord_t<T>::operator const T&() const { return value; }

template <typename T>
coord_t<T>::operator T&() { return value; }

template <typename T>
coord_t<T>::operator double() const { return static_cast<double>(value); }

template <typename T>
coord_t<T> coord_t<T>::operator+(const coord_t& other) const {
    return coord_t(value + other.value);
}

template <typename T>
coord_t<T> coord_t<T>::operator-(const coord_t& other) const {
    return coord_t(value - other.value);
}

template <typename T>
coord_t<T> coord_t<T>::operator*(const coord_t& other) const {
    return coord_t(value * other.value);
}

template <typename T>
coord_t<T> coord_t<T>::operator/(const coord_t& other) const {
    static_assert(!std::is_integral_v<T> || other.value != 0, "Division by zero for integral types");
    assert(other.value != 0 && "Division by zero");
    
    return coord_t(value / other.value);
}

template <typename T>
coord_t<T> coord_t<T>::operator+(T scalar) const {
    return coord_t(value + scalar);
}

template <typename T>
coord_t<T> coord_t<T>::operator-(T scalar) const {
    return coord_t(value - scalar);
}

template <typename T>
coord_t<T> coord_t<T>::operator*(T scalar) const {
    return coord_t(value * scalar);
}

template <typename T>
coord_t<T> coord_t<T>::operator/(T scalar) const {
    static_assert(!std::is_integral_v<T> || scalar != 0, "Division by zero for integral types");
    assert(scalar != 0 && "Division by zero");
    return coord_t(value / scalar);
}

template <typename T>
coord_t<T>& coord_t<T>::operator+=(const coord_t& other) {
    value += other.value;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator-=(const coord_t& other) {
    value -= other.value;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator*=(const coord_t& other) {
    value *= other.value;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator/=(const coord_t& other) {
    static_assert(!std::is_integral_v<T> || other.value != 0, "Division by zero for integral types");
    assert(other.value != 0 && "Division by zero");
    value /= other.value;
    
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator+=(T scalar) {
    value += scalar;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator-=(T scalar) {
    value -= scalar;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator*=(T scalar) {
    value *= scalar;
    return *this;
}

template <typename T>
coord_t<T>& coord_t<T>::operator/=(T scalar) {
    static_assert(!std::is_integral_v<T> || scalar != 0, "Division by zero for integral types");
    assert(scalar != 0 && "Division by zero");
    value /= scalar;
    
    return *this;
}

template <typename T>
bool coord_t<T>::operator==(const coord_t& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(static_cast<double>(value) - static_cast<double>(other.value)) < TOLERANCE;
    } 
    else {
        return value == other.value;
    }
}

template <typename T>
bool coord_t<T>::operator!=(const coord_t& other) const {
    return !(*this == other);
}

template <typename T>
bool coord_t<T>::operator<(const coord_t& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return (static_cast<double>(value) + TOLERANCE) < static_cast<double>(other.value);
    } 
    else {
        return value < other.value;
    }
}

template <typename T>
bool coord_t<T>::operator<=(const coord_t& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return (static_cast<double>(value) - static_cast<double>(other.value)) <= TOLERANCE;
    } 
    else {
        return value <= other.value;
    }
}

template <typename T>
bool coord_t<T>::operator>(const coord_t& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return (static_cast<double>(value) - TOLERANCE) > static_cast<double>(other.value);
    } 
    else {
        return value > other.value;
    }
}

template <typename T>
bool coord_t<T>::operator>=(const coord_t& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return (static_cast<double>(other.value) - static_cast<double>(value)) <= TOLERANCE;
    } 
    else {
        return value >= other.value;
    }
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const coord_t<T>& coord) {
    return os << coord.value;
}

template <typename T>
coord_t<T> operator+(T scalar, const coord_t<T>& coord) {
    return coord_t<T>(scalar + coord.value);
}

template <typename T>
coord_t<T> operator-(T scalar, const coord_t<T>& coord) {
    return coord_t<T>(scalar - coord.value);
}

template <typename T>
coord_t<T> operator*(T scalar, const coord_t<T>& coord) {
    return coord_t<T>(scalar * coord.value);
}

template <typename T>
coord_t<T> operator/(T scalar, const coord_t<T>& coord) {
    static_assert(!std::is_integral_v<T> || coord.value != 0, "Division by zero for integral types");
    assert(coord.value != 0 && "Division by zero");
    
    return coord_t<T>(scalar / coord.value);
}

#endif // COORDINATE_TPP