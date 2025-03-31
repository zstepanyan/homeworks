#include "set.hpp"

template <typename T>
Set<T>::Node::Node(T data, COLOR color, Node* left, Node* right, Node* parent)
    : data(std::move(data)), color(color), left(left), right(right), parent(parent) {}


template <typename T>
Set<T>::Set() : root_(nullptr) {}

template <typename T>
Set<T>::~Set() {
    delete_tree(root_);
}

template <typename T>
void Set<T>::insert(T key) {
    Node* node = new Node(key);
    Node* parent = nullptr;
    Node* curr = root_;

    while (curr != nullptr) {
        parent = curr;
        if (node->data < curr->data) {
            curr = curr->left;
        } 
        else {
            curr = curr->right;
        }
    }

    node->parent = parent;
    if (parent == nullptr) {
        root_ = node;
    } 
    else if (node->data < parent->data) {
        parent->left = node;
    } 
    else {
        parent->right = node;
    }

    fix_insert(node);
}

template <typename T>
void Set<T>::remove(T key) {
    Node* node = root_;
    Node* z = nullptr;
    Node* y = nullptr;
    Node* x = nullptr;

    while (node != nullptr) {
        if (node->data == key) {
            z = node;
        }
        if (node->data <= key) {
            node = node->right;
        } 
        else {
            node = node->left;
        }
    }

    if (z == nullptr) {
        std::cout << "Key not found in the tree" << std::endl;
        return;
    }

    y = z;
    COLOR y_original_color = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(root_, z, z->right);
    } 
    else if (z->right == nullptr) {
        x = z->left;
        transplant(root_, z, z->left);
    } 
    else {
        y = min_value_node(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } 
        else {
            transplant(root_, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(root_, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;
    if (y_original_color == COLOR::BLACK) {
        fix_delete(x);
    }
}


template <typename T>
void Set<T>::rotate_left(Node*& node) {
    Node* child = node->right;
    node->right = child->left;

    if (node->right != nullptr) {
        node->right->parent = node;
    }

    child->parent = node->parent;
    if (node->parent == nullptr) {
        root_ = child;
    } 
    else if (node == node->parent->left) {
        node->parent->left = child;
    } 
    else {
        node->parent->right = child;
    }

    child->left = node;
    node->parent = child;
}

template <typename T>
void Set<T>::rotate_right(Node*& node) {
    Node* child = node->left;
    node->left = child->right;

    if (node->left != nullptr) {
        node->left->parent = node;
    }

    child->parent = node->parent;
    if (node->parent == nullptr) {
        root_ = child;
    } 
    else if (node == node->parent->left) {
        node->parent->left = child;
    } 
    else {
        node->parent->right = child;
    }

    child->right = node;
    node->parent = child;
}

template <typename T>
void Set<T>::fix_insert(Node*& node) {
    Node* parent = nullptr;
    Node* grandparent = nullptr;

    while (node != root_ && node->color == COLOR::RED && node->parent->color == COLOR::RED) {
        parent = node->parent;
        grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle != nullptr && uncle->color == COLOR::RED) {
                grandparent->color = COLOR::RED;
                parent->color = COLOR::BLACK;
                uncle->color = COLOR::BLACK;
                node = grandparent;
            } 
            else {
                if (node == parent->right) {
                    rotate_left(parent);
                    node = parent;
                    parent = node->parent;
                }

                rotate_right(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        } 
        else {
            Node* uncle = grandparent->left;
            if (uncle != nullptr && uncle->color == COLOR::RED) {
                grandparent->color = COLOR::RED;
                uncle->color = COLOR::BLACK;
                parent->color = COLOR::BLACK;
                node = grandparent;
            } 
            else {
                if (node == parent->left) {
                    rotate_right(parent);
                    node = parent;
                    parent = node->parent;
                }

                rotate_left(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }

    root_->color = COLOR::BLACK;
}

template <typename T>
void Set<T>::fix_delete(Node*& node) {
    while (node != root_ && node->color == COLOR::BLACK) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            if (sibling->color == COLOR::RED) {
                sibling->color = COLOR::BLACK;
                node->parent->color = COLOR::RED;
                rotate_left(node->parent);
                sibling = node->parent->right;
            }

            if ((sibling->left == nullptr || sibling->left->color == COLOR::BLACK) &&
                (sibling->right == nullptr || sibling->right->color == COLOR::BLACK)) {
                sibling->color = COLOR::RED;
                node = node->parent;
            } 
            else {
                if (sibling->right == nullptr || sibling->right->color == COLOR::BLACK) {
                    if (sibling->left != nullptr) {
                        sibling->left->color = COLOR::RED;
                    }

                    sibling->color = COLOR::RED;
                    rotate_right(sibling);
                    sibling = node->parent->right;
                }

                sibling->color = node->parent->color;
                node->parent->color = COLOR::BLACK;
                if (sibling->right != nullptr) {
                    sibling->right->color = COLOR::BLACK;
                }

                rotate_left(node->parent);
                node = root_;
            }
        } else {
            Node* sibling = node->parent->left;
            if (sibling->color == COLOR::RED) {
                sibling->color = COLOR::BLACK;
                node->parent->color = COLOR::RED;
                rotate_right(node->parent);
                sibling = node->parent->left;
            }

            if ((sibling->left == nullptr || sibling->left->color == COLOR::BLACK) &&
                (sibling->right == nullptr || sibling->right->color == COLOR::BLACK)) {
                sibling->color = COLOR::RED;
                node = node->parent;
            } 
            else {
                if (sibling->left == nullptr || sibling->left->color == COLOR::BLACK) {
                    if (sibling->right != nullptr) {
                        sibling->right->color = COLOR::BLACK;
                    }

                    sibling->color = COLOR::RED;
                    rotate_left(sibling);
                    sibling = node->parent->left;
                }

                sibling->color = node->parent->color;
                node->parent->color = COLOR::BLACK;
                if (sibling->left != nullptr) {
                    sibling->left->color = COLOR::BLACK;
                }

                rotate_right(node->parent);
                node = root_;
            }
        }
    }

    node->color = COLOR::BLACK;
}

template <typename T>
typename Set<T>::Node* Set<T>::min_value_node(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }

    return current;
}

template <typename T>
void Set<T>::transplant(Node*& root, Node*& u, Node*& v) {
    if (u->parent == nullptr) {
        root = v;
    } 
    else if (u == u->parent->left) {
        u->parent->left = v;
    } 
    else {
        u->parent->right = v;
    }

    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <typename T>
void Set<T>::delete_tree(Node* node) {
    if (node != nullptr) {
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }
}
