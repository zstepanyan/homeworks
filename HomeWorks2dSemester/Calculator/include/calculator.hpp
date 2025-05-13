#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "calculator_error.hpp"
#include <string>
#include <stdexcept>

class calculator {
public:
    calculator() = default;
    double evaluate_expression(const std::string& expr) const;

private:
    lexer lexer_;
    parser parser_;
};

#endif // CALCULATOR_HPP