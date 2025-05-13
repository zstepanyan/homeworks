#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class token {
public:
    enum class type {
        number,
        operator_,
        lparen,
        rparen, 
        function
    };

    token(type t, const std::string& v);
    type get_type() const;
    std::string get_value() const;

private:
    type type_;
    std::string value_;
};


#endif // TOKEN_HPP