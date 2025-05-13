#ifndef OPERATOR_NODE_HPP
#define OPERATOR_NODE_HPP

#include "expression_node.hpp"
#include <memory>   

class operator_node : public expression_node {
public:
    // operator_node(char op, expression_node* left, expression_node* right);
    operator_node(char op, std::shared_ptr<expression_node> left, std::shared_ptr<expression_node> right);
    double evaluate() const override;
    
private:
    std::shared_ptr<expression_node> left_;
    std::shared_ptr<expression_node> right_;
    // expression_node* left_;
    // expression_node* right_;
    char op_;
};

#endif // OPERATOR_NODE_HPP