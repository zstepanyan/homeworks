#include "../../include/ast/function_node.hpp"
#include "../../include/calculator_error.hpp"
#include <cmath>
#include <stdexcept>

function_node::function_node(const std::string& func_name, std::shared_ptr<expression_node> arg)
    : func_name_(func_name), arg_(arg) {}

double function_node::evaluate() const {
    double arg_val = arg_->evaluate();
    if (func_name_ == "sqrt") {
        if (arg_val < 0) {
            throw calculator_error("Square root of negative number");
        }
        
        return std::sqrt(arg_val);
    }
    if (func_name_ == "log2") {
        if (arg_val <= 0) {
            throw calculator_error("Logarithm of non-positive number");
        }
        
        return std::log2(arg_val);
    }
    if (func_name_ == "ln") {
        if (arg_val <= 0) {
            throw calculator_error("Logarithm of non-positive number");
        }
        
        return std::log(arg_val);
    }
    if (func_name_ == "log10") {
        if (arg_val <= 0) {
            throw calculator_error("Logarithm of non-positive number");
        }
        
        return std::log10(arg_val);
    }
    if (func_name_ == "sin") {
        return std::sin(arg_val);
    }
    if (func_name_ == "cos") {
        return std::cos(arg_val);
    }
    if (func_name_ == "tan") {
        double cos_val = std::cos(arg_val);
        if (std::abs(cos_val) < 1e-10) {
            throw calculator_error("Tangent undefined");
        }
        
        return std::tan(arg_val);
    }
    if (func_name_ == "cot") {
        double sin_val = std::sin(arg_val);
        if (std::abs(sin_val) < 1e-10) {
            throw calculator_error("Cotangent undefined");
        }
        
        return 1.0 / std::tan(arg_val);
    }
    
    throw calculator_error("Unknown function: " + func_name_);
}