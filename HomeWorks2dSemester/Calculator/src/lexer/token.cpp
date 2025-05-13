#include "../../include/lexer/token.hpp"

token::token(type t, const std::string &v): type_(t), value_(v) {}

token::type token::get_type() const
{
    return type_;
}

std::string token::get_value() const 
{
    return value_;
}
