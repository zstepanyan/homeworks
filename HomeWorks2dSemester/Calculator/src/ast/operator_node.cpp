#include "../../include/ast/operator_node.hpp"
#include <stdexcept>

operator_node::operator_node(char op, std::shared_ptr<expression_node> left, std::shared_ptr<expression_node> right)
    : op_(op), left_(left), right_(right) {}

double operator_node::evaluate() const {
    double left_val = left_->evaluate();
    double right_val = right_->evaluate();
    switch (op_) {
        case '+': 
            return left_val + right_val;
        case '-': 
            return left_val - right_val;
        case '*': 
            return left_val * right_val;
        case '/':
            if (right_val == 0) {
                throw std::runtime_error("Division by zero");
            }
            
            return left_val / right_val;
        default: 
            throw std::runtime_error("Invalid operator: " + std::string(1, op_));
    }
}