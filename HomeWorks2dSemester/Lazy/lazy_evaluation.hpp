#ifndef LAZY_COMPUTATION_H
#define LAZY_COMPUTATION_H

#include <vector>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <functional>

template <typename T>
class VectorPairOps {
public:
    // Constructors
    VectorPairOps(const std::vector<T>& left, const std::vector<T>& right);
    VectorPairOps(std::vector<T>&& left, std::vector<T>&& right);

    // Index-based operations
    T element_sum(std::size_t ind) const;
    T element_diff(std::size_t ind) const;
    T element_product(std::size_t ind) const;
    T element_division(std::size_t ind) const;

    // Accumulation operations
    T sum_of_elements() const;
    T difference_of_elements() const;

    // Container operations
    std::vector<T> container_of_sums() const;
    std::vector<T> container_of_differences() const;
    std::vector<T> container_of_max() const;

    // Generic operation
    template<typename BinaryOp>
    std::vector<T> combine_vectors(BinaryOp op) const;

    // Utility methods
    std::size_t size() const;
    bool empty() const;

private:
    std::vector<T> left_;
    std::vector<T> right_;
};

// Template methods need to be defined in the header
template<typename T>
template<typename BinaryOp>
std::vector<T> VectorPairOps<T>::combine_vectors(BinaryOp op) const {
    std::vector<T> result;
    result.reserve(left_.size());
    std::transform(left_.begin(), left_.end(), right_.begin(), std::back_inserter(result), op);
    return result;
}

#endif // LAZY_COMPUTATION_H