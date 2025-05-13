#ifndef NUMBER_NODE_HPP
#define NUMBER_NODE_HPP

#include "expression_node.hpp"

class number_node : public expression_node {
public:
    explicit number_node(double value) : value_(value) {} 
    double evaluate() const override;

private:
    double value_;
};

#endif // NUMBER_NODE_HPP