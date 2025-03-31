#ifndef SET_HPP
#define SET_HPP

#include <utility>

template <typename T>
class Set {
public:
    enum class COLOR { RED, BLACK };

    Set();  
    ~Set();
    void insert(T key);
    void remove(T key);

private:
    struct Node {
        T data;
        COLOR color;
        Node* left;
        Node* right;
        Node* parent;

        Node(T data, COLOR color = COLOR::RED, Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr)
            : data(std::move(data)), color(color), left(left), right(right), parent(parent) {}
    };

private:
    Node* root_;

    void rotate_left(Node*& node);
    void rotate_right(Node*& node);
    void fix_insert(Node*& node);
    void fix_delete(Node*& node);
    Node* min_value_node(Node* node);
    void transplant(Node*& root, Node*& u, Node*& v);
    void delete_tree(Node* node);
};

#endif // SET_HPP
