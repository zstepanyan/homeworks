#ifndef VECTOR_TPP
#define VECTOR_TPP

#include "../include/Vector.hpp"
#include <cmath>
#include <cassert>
#include <type_traits>

template <typename T, std::size_t N>
typename Vector<T, N>::iterator Vector<T, N>::begin() { return data.begin(); }

template <typename T, std::size_t N>
typename Vector<T, N>::iterator Vector<T, N>::end() { return data.end(); }

template <typename T, std::size_t N>
typename Vector<T, N>::const_iterator Vector<T, N>::begin() const { return data.begin(); }

template <typename T, std::size_t N>
typename Vector<T, N>::const_iterator Vector<T, N>::end() const { return data.end(); }

template <typename T, std::size_t N>
typename Vector<T, N>::const_iterator Vector<T, N>::cbegin() const { return data.cbegin(); }

template <typename T, std::size_t N>
typename Vector<T, N>::const_iterator Vector<T, N>::cend() const { return data.cend(); }

template <typename T, std::size_t N>
Vector<T, N>::Vector() {
    for (auto& elem : data) {
        elem = coord_t<T>();
    }
}

template <typename T, std::size_t N>
Vector<T, N>::Vector(const std::array<T, N>& arr) {
    for (std::size_t i = 0; i < N; ++i) {
        data[i] = coord_t<T>(arr[i]);
    }
}

template <typename T, std::size_t N>
Vector<T, N>::Vector(const Vector& other) : data(other.data) {}

template <typename T, std::size_t N>
Vector<T, N>::Vector(Vector&& other) noexcept : data(std::move(other.data)) {}

template <typename T, std::size_t N>
Vector<T, N>::Vector(std::initializer_list<T> il) {
    assert(il.size() == N && "Initializer list size must match dimension N");
    auto it = il.begin();
    for (std::size_t i = 0; i < N; ++i) {
        data[i] = coord_t<T>(*it++);
    }
}

template <typename T, std::size_t N>
Vector<T, N>::Vector(const Vector& p2, const Vector& p1) {
    for (std::size_t i = 0; i < N; ++i) {
        data[i] = coord_t<T>(p2.data[i].value - p1.data[i].value);
    }
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator=(const Vector& other) {
    if (this != &other) {
        data = other.data;
    }

    return *this;
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
    }
    
    return *this;
}

template <typename T, std::size_t N>
coord_t<T>& Vector<T, N>::operator[](std::size_t idx) {
    assert(idx < N && "Index out of bounds");
    return data.at(idx);
}

template <typename T, std::size_t N>
const coord_t<T>& Vector<T, N>::operator[](std::size_t idx) const {
    assert(idx < N && "Index out of bounds");
    return data.at(idx);
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator+=(const Vector& other) {
    for (std::size_t i = 0; i < N; ++i) {
        data[i].value += other.data[i].value;
    }
    
    return *this;
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator-=(const Vector& other) {
    for (std::size_t i = 0; i < N; ++i) {
        data[i].value -= other.data[i].value;
    }
    
    return *this;
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator+(const Vector& other) const {
    Vector result = *this;
    result += other;
    
    return result;
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-(const Vector& other) const {
    Vector result = *this;
    result -= other;
    
    return result;
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-() const {
    Vector result;
    for (std::size_t i = 0; i < N; ++i) {
        result.data[i].value = -data[i].value;
    }
    
    return result;
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator*=(T scalar) {
    for (auto& elem : data) {
        elem.value *= scalar;
    }
    
    return *this;
}

template <typename T, std::size_t N>
Vector<T, N>& Vector<T, N>::operator/=(T scalar) {
    static_assert(!std::is_integral_v<T> || scalar != 0, "Division by zero for integral types");
    assert(scalar != 0 && "Division by zero");
    for (auto& elem : data) {
        elem.value /= scalar;
    }
    
    return *this;
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*(T scalar) const {
    Vector result = *this;
    result *= scalar;
    
    return result;
}

template <typename T, std::size_t N>
Vector<T, N> operator*(T scalar, const Vector<T, N>& vec) {
    return vec * scalar;
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/(T scalar) const {
    Vector result = *this;
    result /= scalar;
    
    return result;
}

template <typename T, std::size_t N>
bool Vector<T, N>::operator==(const Vector<T, N>& other) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (data[i] != other[i]) {
            return false;
        }
    }

    return true;
} 

template <typename T, std::size_t N>
bool Vector<T, N>::operator!=(const Vector<T, N>& other) const {
    return !(*this == other);
} 

template <typename T, std::size_t N>
T Vector<T, N>::dot(const Vector& other) const {
    T result = T{};
    for (std::size_t i = 0; i < N; ++i) {
        result += data[i].value * other.data[i].value;
    }
    
    return result;
}

template <typename T, std::size_t N>
T Vector<T, N>::cross(const Vector& other) const {
    static_assert(N == 2 || N == 3, "Cross product numerical value only defined for N=2 or N=3");
    if constexpr (N == 2) {
        return data[0].value * other.data[1].value - data[1].value * other.data[0].value;
    } 
    else {
        T i = data[1].value * other.data[2].value - data[2].value * other.data[1].value;
        T j = data[2].value * other.data[0].value - data[0].value * other.data[2].value;
        T k = data[0].value * other.data[1].value - data[1].value * other.data[0].value;
    
        return std::sqrt(i*i + j*j + k*k);
    }
}

template <typename T, std::size_t N>
double Vector<T, N>::length() const {
    return std::sqrt(static_cast<double>(dot(*this)));
}

template <typename T, std::size_t N>
double Vector<T, N>::angle_between(const Vector& other) const {
    double dot_prod = static_cast<double>(dot(other));
    double len1 = length();
    double len2 = other.length();
    if (len1 == 0 || len2 == 0) {
        return 0.0;
    }

    double cos_theta = dot_prod / (len1 * len2);
    cos_theta = std::max(-1.0, std::min(1.0, cos_theta));
    
    return std::acos(cos_theta);
}

template <typename T, std::size_t N>
Vector<T, N> Vector<T, N>::project_onto(const Vector& other) const {
    T dot_prod = dot(other);
    T other_sq = other.dot(other);
    if (other_sq == 0) {
        return Vector();
    }
    
    return other * (dot_prod / other_sq);
}

template <typename T, std::size_t N>
bool Vector<T, N>::parallel(const Vector& other) const {
    // Check if either vector is zero
    bool this_zero = true, other_zero = true;
    for (std::size_t i = 0; i < N; ++i) {
        if (std::abs(static_cast<double>(data[i].value)) > coord_t<T>::TOLERANCE) {
            this_zero = false;
        }
        if (std::abs(static_cast<double>(other.data[i].value)) > coord_t<T>::TOLERANCE) {
            other_zero = false;
        }
    }

    if (this_zero || other_zero) {
        return true; // Zero vector is parallel to any vector
    }

    // For N=1, check if components are equal or proportional
    if constexpr (N == 1) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::abs(static_cast<double>(data[0].value) - static_cast<double>(other.data[0].value)) < coord_t<T>::TOLERANCE;
        } 
        else {
            return data[0].value == other.data[0].value;
        }
    }

    // For N>1, check proportionality using cross-ratio
    // Find first non-zero component pair to establish ratio
    double ratio = 0.0;
    bool ratio_set = false;
    for (std::size_t i = 0; i < N; ++i) {
        if (std::abs(static_cast<double>(other.data[i].value)) > coord_t<T>::TOLERANCE) {
            ratio = static_cast<double>(data[i].value) / static_cast<double>(other.data[i].value);
            ratio_set = true;
            break;
        }
    }
    if (!ratio_set) {
        return false; // No non-zero component to establish ratio
    }

    // Check if all components follow the same ratio
    for (std::size_t i = 0; i < N; ++i) {
        if (std::abs(static_cast<double>(other.data[i].value)) > coord_t<T>::TOLERANCE) {
            double current_ratio = static_cast<double>(data[i].value) / static_cast<double>(other.data[i].value);
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(current_ratio - ratio) > coord_t<T>::TOLERANCE) {
                    return false;
                }
            } 
            else {
                if (current_ratio != ratio) {
                    return false;
                }
            }
        } 
        else if (std::abs(static_cast<double>(data[i].value)) > coord_t<T>::TOLERANCE) {
            return false; // Non-zero component in this where other is zero
        }
    }
    return true;
}

#endif // VECTOR_TPP