#include "../../include/parser/parser.hpp"
#include "../../include/calculator_error.hpp"
#include <stdexcept>
#include <iostream>

std::shared_ptr<expression_node> parser::parse(const std::vector<token>& tokens) const {
    if (tokens.empty()) { 
        throw calculator_error("No valid tokens");
    }

    size_t pos = 0;
    auto tree = parse_expression(tokens, pos);
    if (pos < tokens.size()) {
        throw calculator_error("Unexpected tokens after expression");
    }
    
    return tree;
}

std::shared_ptr<expression_node> parser::parse_expression(const std::vector<token>& tokens, size_t& pos) const {
    auto left = parse_term(tokens, pos);
    while (pos < tokens.size() && tokens[pos].get_type() == token::type::operator_ &&
           (tokens[pos].get_value() == "+" || tokens[pos].get_value() == "-")) {
        char op = tokens[pos].get_value()[0];
        ++pos;
        auto right = parse_term(tokens, pos);
        left = std::make_shared<operator_node>(op, left, right);
    }
    
    return left;
}

std::shared_ptr<expression_node> parser::parse_term(const std::vector<token>& tokens, size_t& pos) const {
    auto left = parse_power(tokens, pos);
    while (pos < tokens.size() && tokens[pos].get_type() == token::type::operator_ &&
           (tokens[pos].get_value() == "*" || tokens[pos].get_value() == "/")) {
        char op = tokens[pos].get_value()[0];
        ++pos;
        auto right = parse_power(tokens, pos);
        left = std::make_shared<operator_node>(op, left, right);
    }
    
    return left;
}

std::shared_ptr<expression_node> parser::parse_power(const std::vector<token>& tokens, size_t& pos) const {
    auto left = parse_factor(tokens, pos);
    while (pos < tokens.size() && tokens[pos].get_type() == token::type::operator_ &&
           tokens[pos].get_value() == "^") {
        char op = tokens[pos].get_value()[0];
        ++pos;
        auto right = parse_factor(tokens, pos);
        left = std::make_shared<operator_node>(op, left, right);
    }
    
    return left;
}

std::shared_ptr<expression_node> parser::parse_factor(const std::vector<token>& tokens, size_t& pos) const {
    if (pos >= tokens.size()) {
        throw calculator_error("Unexpected end of expression");
    }
    
    const auto& token = tokens[pos];
    std::cerr << "Parsing token: type=" << static_cast<int>(token.get_type()) << ", value=" << token.get_value() << std::endl; // Debug
    if (token.get_type() == token::type::number) {
        ++pos;
        return std::make_shared<number_node>(std::stod(token.get_value()));
    }
    if (token.get_type() == token::type::lparen) {
        ++pos;
        auto expr = parse_expression(tokens, pos);
        if (pos >= tokens.size() || tokens[pos].get_type() != token::type::rparen) {
            throw calculator_error("Expected closing parenthesis");
        }
        
        ++pos;
        return expr;
    }
    if (token.get_type() == token::type::function) {
        std::string func_name = token.get_value();
        ++pos;
        if (pos >= tokens.size() || tokens[pos].get_type() != token::type::lparen) {
            throw calculator_error("Expected opening parenthesis after function");
        }
    
        ++pos;
        auto arg = parse_expression(tokens, pos);
        if (pos >= tokens.size() || tokens[pos].get_type() != token::type::rparen) {
            throw calculator_error("Expected closing parenthesis after function argument");
        }
        ++pos;
        return std::make_shared<function_node>(func_name, arg);
    }
    
    throw calculator_error("Expected number, parenthesis, or function, got: " + token.get_value());
}