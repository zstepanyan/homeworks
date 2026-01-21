#ifndef SEGMENTTREE_HPP
#define SEGMENTTREE_HPP

#include <vector>
#include <functional>
#include <cstddef>
#include <type_traits>
#include <limits>
#include <memory>

// Node structure for balanced binary tree
template <typename T>
struct SegmentTreeNode {
    T value;
    T lazy;
    bool has_lazy = false;
    std::size_t left_idx;   // Range start
    std::size_t right_idx;  // Range end
    std::unique_ptr<SegmentTreeNode> left;
    std::unique_ptr<SegmentTreeNode> right;

    SegmentTreeNode(std::size_t l, std::size_t r, T val)
        : value(val), lazy{}, has_lazy(false), left_idx(l), right_idx(r) {}

    bool is_leaf() const { return left_idx == right_idx; }
    std::size_t mid() const { return left_idx + (right_idx - left_idx) / 2; }
};

template <typename T, typename Op = std::plus<T>>
class SegmentTree {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

public:
    using value_type = T;
    using size_type = std::size_t;
    using operation_type = Op;
    using node_type = SegmentTreeNode<T>;

    SegmentTree() = default;
    explicit SegmentTree(size_type n, T identity = T{});
    explicit SegmentTree(const std::vector<T>& data, T identity = T{});

    SegmentTree(const SegmentTree& other);
    SegmentTree& operator=(const SegmentTree& other);
    SegmentTree(SegmentTree&&) noexcept = default;
    SegmentTree& operator=(SegmentTree&&) noexcept = default;

    // Build tree from data
    void build(const std::vector<T>& data);

    // Point update: set value at index
    void update(size_type index, T value);

    // Point update: modify value at index using operation
    void modify(size_type index, T delta);

    // Range update: add delta to all elements in [left, right]
    void range_update(size_type left, size_type right, T delta);

    // Range query: apply operation over [left, right]
    T query(size_type left, size_type right) const;

    // Single element access
    T get(size_type index) const;

    // Size of the underlying array
    size_type size() const;

    // Check if empty
    bool empty() const;

    // Clear the tree
    void clear();

    // Resize and reinitialize
    void resize(size_type n, T identity = T{});

    // Tree height (for balance verification)
    size_type height() const;

private:
    std::unique_ptr<node_type> root_;
    size_type n_ = 0;
    T identity_;
    Op op_;

    std::unique_ptr<node_type> build_tree(const std::vector<T>& data, size_type left, size_type right);
    std::unique_ptr<node_type> clone_tree(const node_type* node) const;
    void push_down(node_type* node) const;
    void update_internal(node_type* node, size_type index, T value);
    void range_update_internal(node_type* node, size_type left, size_type right, T delta);
    T query_internal(node_type* node, size_type left, size_type right) const;
    size_type height_internal(const node_type* node) const;
    size_type range_length(const node_type* node) const;
};

// Common segment tree types
template <typename T>
using SumSegmentTree = SegmentTree<T, std::plus<T>>;

template <typename T>
struct MinOp {
    T operator()(const T& a, const T& b) const { return std::min(a, b); }
};

template <typename T>
struct MaxOp {
    T operator()(const T& a, const T& b) const { return std::max(a, b); }
};

template <typename T>
using MinSegmentTree = SegmentTree<T, MinOp<T>>;

template <typename T>
using MaxSegmentTree = SegmentTree<T, MaxOp<T>>;

#include "../src/SegmentTree.tpp"

#endif // SEGMENTTREE_HPP
