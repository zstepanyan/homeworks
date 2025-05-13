#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <cctype>
#include <vector>
#include "token.hpp"

class lexer {
public:
    lexer() = default;
    std::vector<token> tokenize(const std::string& expression) const;
};

#endif // LEXER_HPP