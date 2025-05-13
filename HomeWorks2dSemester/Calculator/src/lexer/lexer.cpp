#include "../../include/lexer/lexer.hpp"
#include "../../include/calculator_error.hpp"
#include <cctype>
#include <stdexcept>
#include <set>

const std::set<std::string> valid_functions = {
    "sqrt", "log2", "ln", "log10", "sin", "cos", "tan", "cot"
};

std::vector<token> lexer::tokenize(const std::string& expr) const {
    std::vector<token> tokens;
    size_t i = 0;
    while (i < expr.length()) {
        char c = expr[i];
        if (std::isspace(c)) {
            ++i;
            continue;
        }
        if (std::isdigit(c) || c == '.') {
            std::string num;
            bool has_decimal = false;
            while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (has_decimal) {
                        throw calculator_error("Invalid number format");
                    }
                    has_decimal = true;
                }
                
                num += expr[i];
                ++i;
            }
            try {
                std::stod(num);
                tokens.emplace_back(token::type::number, num);
            } catch (...) {
                throw calculator_error("Invalid number: " + num);
            }
            
            continue;
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            tokens.emplace_back(token::type::operator_, std::string(1, c));
            ++i;
            continue;
        }
        if (c == '(') {
            tokens.emplace_back(token::type::lparen, "(");
            ++i;
            continue;
        }
        if (c == ')') {
            tokens.emplace_back(token::type::rparen, ")");
            ++i;
            continue;
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            std::string func;
            while (i < expr.length() && ((expr[i] >= 'a' && expr[i] <= 'z') || (expr[i] >= 'A' && expr[i] <= 'Z'))) {
                func += expr[i];
                ++i;
            }
            
            if (valid_functions.count(func)) {
                tokens.emplace_back(token::type::function, func);
            } 
            else {
                throw calculator_error("Unknown function: " + func);
            }
            
            continue;
        }
        
        throw calculator_error("Invalid character: " + std::string(1, c));
    }
    
    return tokens;
}