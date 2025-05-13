#include "../include/calculator.hpp"

double calculator::evaluate_expression(const std::string &expr) const
{
    if (expr.empty()) {
        throw std::runtime_error("Empty expression");
    }

    auto tokens = lexer_.tokenize(expr);
    auto tree = parser_.parse(tokens);

    return tree->evaluate();
}