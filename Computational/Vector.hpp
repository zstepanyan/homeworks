#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <initializer_list>
#include <type_traits>
#include "Coordinate.hpp"

template <typename T, std::size_t N>
class Vector {
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");
    static_assert(N > 0, "N must be greater than 0");

public:
    using iterator = typename std::array<coord_t<T>, N>::iterator;
    using const_iterator = typename std::array<coord_t<T>, N>::const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    Vector();
    explicit Vector(const std::array<T, N>& arr);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector(std::initializer_list<T> il);
    Vector(const Vector& p2, const Vector& p1);

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    coord_t<T>& operator[](std::size_t idx);
    const coord_t<T>& operator[](std::size_t idx) const;

    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator-() const;

    Vector& operator*=(T scalar);
    Vector& operator/=(T scalar);
    Vector operator*(T scalar) const;
    Vector operator/(T scalar) const;
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;

    T dot(const Vector& other) const;
    T cross(const Vector& other) const;
    double length() const;
    double angle_between(const Vector& other) const;
    Vector project_onto(const Vector& other) const;
    bool parallel(const Vector& other) const;

private:
    std::array<coord_t<T>, N> data;
};

template <typename T, std::size_t N>
Vector<T, N> operator*(T scalar, const Vector<T, N>& vec);

#include "Vector.tpp"

#endif // VECTOR_HPP