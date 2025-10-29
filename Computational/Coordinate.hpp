#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <type_traits>
#include <ostream>

template <typename T>
struct coord_t {
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

    T value;
    static constexpr double TOLERANCE = 1e-9;

    coord_t();
    coord_t(T val);

    operator const T&() const;
    operator T&();
    operator double() const;

    
    coord_t operator+(const coord_t& other) const;
    coord_t operator-(const coord_t& other) const;
    coord_t operator*(const coord_t& other) const;
    coord_t operator/(const coord_t& other) const;
    coord_t operator+(T scalar) const;
    coord_t operator-(T scalar) const;
    coord_t operator*(T scalar) const;
    coord_t operator/(T scalar) const;
    coord_t& operator+=(const coord_t& other);
    coord_t& operator-=(const coord_t& other);
    coord_t& operator*=(const coord_t& other);
    coord_t& operator/=(const coord_t& other);
    coord_t& operator+=(T scalar);
    coord_t& operator-=(T scalar);
    coord_t& operator*=(T scalar);
    coord_t& operator/=(T scalar);

    bool operator==(const coord_t& other) const;
    bool operator!=(const coord_t& other) const;
    bool operator<(const coord_t& other) const;
    bool operator<=(const coord_t& other) const;
    bool operator>(const coord_t& other) const;
    bool operator>=(const coord_t& other) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const coord_t<T>& coord);

template <typename T>
coord_t<T> operator+(T scalar, const coord_t<T>& coord);

template <typename T>
coord_t<T> operator-(T scalar, const coord_t<T>& coord);

template <typename T>
coord_t<T> operator*(T scalar, const coord_t<T>& coord);

template <typename T>
coord_t<T> operator/(T scalar, const coord_t<T>& coord);

#include "Coordinate.tpp"

#endif // COORDINATE_HPP