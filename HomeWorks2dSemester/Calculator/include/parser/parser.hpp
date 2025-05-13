#ifndef PARSER_H
#define PARSER_H

#include "../ast/expression_node.hpp"
#include "../ast/number_node.hpp"
#include "../ast/operator_node.hpp"
#include "../ast/function_node.hpp"
#include "../lexer/token.hpp"
#include <memory>
#include <vector>

class parser {
public:
    parser() = default;
    std::shared_ptr<expression_node> parse(const std::vector<token>& tokens) const;

private:
    std::shared_ptr<expression_node> parse_expression(const std::vector<token>& tokens, size_t& pos) const;
    std::shared_ptr<expression_node> parse_term(const std::vector<token>& tokens, size_t& pos) const;
    std::shared_ptr<expression_node> parse_power(const std::vector<token>& tokens, size_t& pos) const;
    std::shared_ptr<expression_node> parse_factor(const std::vector<token>& tokens, size_t& pos) const;
};

#endif // PARSER_H