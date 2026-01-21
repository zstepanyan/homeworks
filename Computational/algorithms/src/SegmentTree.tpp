#ifndef SEGMENTTREE_TPP
#define SEGMENTTREE_TPP

#include "../include/SegmentTree.hpp"
#include <cassert>
#include <algorithm>

template <typename T, typename Op>
SegmentTree<T, Op>::SegmentTree(size_type n, T identity)
    : n_(n), identity_(identity), op_() {
    if (n_ > 0) {
        std::vector<T> data(n_, identity_);
        root_ = build_tree(data, 0, n_ - 1);
    }
}

template <typename T, typename Op>
SegmentTree<T, Op>::SegmentTree(const std::vector<T>& data, T identity)
    : n_(data.size()), identity_(identity), op_() {
    if (n_ > 0) {
        root_ = build_tree(data, 0, n_ - 1);
    }
}

template <typename T, typename Op>
SegmentTree<T, Op>::SegmentTree(const SegmentTree& other)
    : n_(other.n_), identity_(other.identity_), op_(other.op_) {
    if (other.root_) {
        root_ = clone_tree(other.root_.get());
    }
}

template <typename T, typename Op>
SegmentTree<T, Op>& SegmentTree<T, Op>::operator=(const SegmentTree& other) {
    if (this != &other) {
        n_ = other.n_;
        identity_ = other.identity_;
        op_ = other.op_;
        root_ = other.root_ ? clone_tree(other.root_.get()) : nullptr;
    }
 
    return *this;
}

template <typename T, typename Op>
std::unique_ptr<typename SegmentTree<T, Op>::node_type>
SegmentTree<T, Op>::clone_tree(const node_type* node) const {
    if (!node) return nullptr;

    auto new_node = std::make_unique<node_type>(node->left_idx, node->right_idx, node->value);
    new_node->lazy = node->lazy;
    new_node->has_lazy = node->has_lazy;
    if (node->left) {
        new_node->left = clone_tree(node->left.get());
    }
    if (node->right) {
        new_node->right = clone_tree(node->right.get());
    }

    return new_node;
}

template <typename T, typename Op>
std::unique_ptr<typename SegmentTree<T, Op>::node_type>
SegmentTree<T, Op>::build_tree(const std::vector<T>& data, size_type left, size_type right) {
    auto node = std::make_unique<node_type>(left, right, identity_);
    if (left == right) {
        node->value = data[left];
        return node;
    }

    size_type mid = left + (right - left) / 2;
    node->left = build_tree(data, left, mid);
    node->right = build_tree(data, mid + 1, right);
    node->value = op_(node->left->value, node->right->value);

    return node;
}

template <typename T, typename Op>
void SegmentTree<T, Op>::build(const std::vector<T>& data) {
    n_ = data.size();
    if (n_ > 0) {
        root_ = build_tree(data, 0, n_ - 1);
    }
    else {
        root_.reset();
    }
}

template <typename T, typename Op>
size_t SegmentTree<T, Op>::range_length(const node_type* node) const {
    return node->right_idx - node->left_idx + 1;
}

template <typename T, typename Op>
void SegmentTree<T, Op>::push_down(node_type* node) const {
    if (!node || !node->has_lazy || node->is_leaf()) {
        return;
    }

    // Propagate lazy value to children
    if (node->left) {
        node->left->lazy = node->left->has_lazy
            ? op_(node->left->lazy, node->lazy)
            : node->lazy;
        node->left->has_lazy = true;
        // For sum trees, multiply by range length
        if constexpr (std::is_same_v<Op, std::plus<T>>) {
            node->left->value = op_(node->left->value,
                node->lazy * static_cast<T>(range_length(node->left.get())));
        }
        else {
            node->left->value = op_(node->left->value, node->lazy);
        }
    }

    if (node->right) {
        node->right->lazy = node->right->has_lazy
            ? op_(node->right->lazy, node->lazy)
            : node->lazy;
        node->right->has_lazy = true;
        if constexpr (std::is_same_v<Op, std::plus<T>>) {
            node->right->value = op_(node->right->value,
                node->lazy * static_cast<T>(range_length(node->right.get())));
        }
        else {
            node->right->value = op_(node->right->value, node->lazy);
        }
    }

    node->has_lazy = false;
    node->lazy = T{};
}

template <typename T, typename Op>
void SegmentTree<T, Op>::update(size_type index, T value) {
    assert(index < n_ && "Index out of bounds");
    update_internal(root_.get(), index, value);
}

template <typename T, typename Op>
void SegmentTree<T, Op>::update_internal(node_type* node, size_type index, T value) {
    if (!node) return;

    push_down(node);
    if (node->is_leaf()) {
        node->value = value;
        return;
    }

    size_type mid = node->mid();
    if (index <= mid) {
        update_internal(node->left.get(), index, value);
    }
    else {
        update_internal(node->right.get(), index, value);
    }

    node->value = op_(
        node->left ? node->left->value : identity_,
        node->right ? node->right->value : identity_
    );
}

template <typename T, typename Op>
void SegmentTree<T, Op>::modify(size_type index, T delta) {
    assert(index < n_ && "Index out of bounds");
    T current = get(index);
    update(index, op_(current, delta));
}

template <typename T, typename Op>
void SegmentTree<T, Op>::range_update(size_type left, size_type right, T delta) {
    assert(left <= right && right < n_ && "Invalid range");
    range_update_internal(root_.get(), left, right, delta);
}

template <typename T, typename Op>
void SegmentTree<T, Op>::range_update_internal(node_type* node, size_type left, size_type right, T delta) {
    if (!node || right < node->left_idx || left > node->right_idx) {
        return;
    }

    // Complete overlap
    if (left <= node->left_idx && node->right_idx <= right) {
        node->lazy = node->has_lazy ? op_(node->lazy, delta) : delta;
        node->has_lazy = true;

        if constexpr (std::is_same_v<Op, std::plus<T>>) {
            node->value = op_(node->value, delta * static_cast<T>(range_length(node)));
        }
        else {
            node->value = op_(node->value, delta);
        }

        return;
    }

    push_down(node);

    range_update_internal(node->left.get(), left, right, delta);
    range_update_internal(node->right.get(), left, right, delta);

    node->value = op_(
        node->left ? node->left->value : identity_,
        node->right ? node->right->value : identity_
    );
}

template <typename T, typename Op>
T SegmentTree<T, Op>::query(size_type left, size_type right) const {
    assert(left <= right && right < n_ && "Invalid query range");
    return query_internal(root_.get(), left, right);
}

template <typename T, typename Op>
T SegmentTree<T, Op>::query_internal(node_type* node, size_type left, size_type right) const {
    if (!node || right < node->left_idx || left > node->right_idx) {
        return identity_;
    }

    // Complete overlap
    if (left <= node->left_idx && node->right_idx <= right) {
        return node->value;
    }

    push_down(node);

    T left_result = query_internal(node->left.get(), left, right);
    T right_result = query_internal(node->right.get(), left, right);

    return op_(left_result, right_result);
}

template <typename T, typename Op>
T SegmentTree<T, Op>::get(size_type index) const {
    assert(index < n_ && "Index out of bounds");
    return query(index, index);
}

template <typename T, typename Op>
typename SegmentTree<T, Op>::size_type SegmentTree<T, Op>::size() const {
    return n_;
}

template <typename T, typename Op>
bool SegmentTree<T, Op>::empty() const {
    return n_ == 0;
}

template <typename T, typename Op>
void SegmentTree<T, Op>::clear() {
    root_.reset();
    n_ = 0;
}

template <typename T, typename Op>
void SegmentTree<T, Op>::resize(size_type n, T identity) {
    identity_ = identity;
    n_ = n;
    if (n_ > 0) {
        std::vector<T> data(n_, identity_);
        root_ = build_tree(data, 0, n_ - 1);
    }
    else {
        root_.reset();
    }
}

template <typename T, typename Op>
typename SegmentTree<T, Op>::size_type SegmentTree<T, Op>::height() const {
    return height_internal(root_.get());
}

template <typename T, typename Op>
typename SegmentTree<T, Op>::size_type SegmentTree<T, Op>::height_internal(const node_type* node) const {
    if (!node) {
        return 0
    };
    
    return 1 + std::max(height_internal(node->left.get()), height_internal(node->right.get()));
}

#endif // SEGMENTTREE_TPP
