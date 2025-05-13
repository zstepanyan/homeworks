#ifndef EXPRESSION_NODE_HPP
#define EXPRESSION_NODE_HPP

class expression_node {
public:
    virtual ~expression_node() = default;
    virtual double evaluate() const = 0;
};

#endif // expression_node_hpp