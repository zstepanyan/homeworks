#ifndef JSON_LEXER_HPP
#define JSON_LEXER_HPP

#include <string>
#include <stdexcept>

class JsonLexer {
public:
    enum class TokenType {
        LBrace, RBrace, LBracket, RBracket, Colon, Comma,
        String, Number, True, False, Null, End
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    JsonLexer(const std::string& input);
    Token next_token();

private:
    const std::string& input_;
    size_t pos_;
    
    char peek() const;
    char consume();
    void skip_whitespace();
    Token lex_string();
    Token lex_number();
    Token lex_keyword();
};

#endif // JSON_LEXER_HPP