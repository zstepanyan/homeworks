#include "json_lexer.hpp"
#include <cctype>

JsonLexer::JsonLexer(const std::string& input) : input_(input), pos_(0) {}

JsonLexer::Token JsonLexer::next_token() {
    skip_whitespace();
    if (pos_ >= input_.size()){ 
        return {TokenType::End, ""};
    }

    char c = consume();
    switch (c) {
        case '{': return {TokenType::LBrace, "{"};
        case '}': return {TokenType::RBrace, "}"};
        case '[': return {TokenType::LBracket, "["};
        case ']': return {TokenType::RBracket, "]"};
        case ':': return {TokenType::Colon, ":"};
        case ',': return {TokenType::Comma, ","};
        case '"': return lex_string();
        case '-': case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            --pos_; return lex_number();
        default:
            if (std::isalpha(c)) {
                --pos_; return lex_keyword();
            }
            throw std::runtime_error("Invalid character at position " + std::to_string(pos_));
    }
}

char JsonLexer::peek() const {
    return pos_ < input_.size() ? input_[pos_] : '\0';
}

char JsonLexer::consume() {
    return pos_ < input_.size() ? input_[pos_++] : '\0';
}

void JsonLexer::skip_whitespace() {
    while (pos_ < input_.size() && std::isspace(input_[pos_])) {
        ++pos_;
    }
}

JsonLexer::Token JsonLexer::lex_string() {
    std::string s;
    while (pos_ < input_.size() && input_[pos_] != '"') {
        char c = consume();
        if (c == '\\') {
            if (pos_ >= input_.size()) throw std::runtime_error("Incomplete escape sequence");
            c = consume();
            switch (c) {
                case '"': s += '"'; break;
                case '\\': s += '\\'; break;
                case '/': s += '/'; break;
                case 'b': s += '\b'; break;
                case 'f': s += '\f'; break;
                case 'n': s += '\n'; break;
                case 'r': s += '\r'; break;
                case 't': s += '\t'; break;
                default: throw std::runtime_error("Invalid escape sequence");
            }
        } 
        else {
            s += c;
        }
    }

    if (pos_ >= input_.size() || input_[pos_] != '"') {
        throw std::runtime_error("Unterminated string");
    }
    
    consume(); 
    return {TokenType::String, s};
}

JsonLexer::Token JsonLexer::lex_number() {
    std::string num_str;
    if (peek() == '-'){ 
        num_str += consume();
    }
    
    while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
        num_str += consume();
    }
    
    if (pos_ < input_.size() && input_[pos_] == '.') {
        num_str += consume();
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            num_str += consume();
        }
    }
    
    if (pos_ < input_.size() && (input_[pos_] == 'e' || input_[pos_] == 'E')) {
        num_str += consume();
        if (pos_ < input_.size() && (input_[pos_] == '+' || input_[pos_] == '-')) {
            num_str += consume();
        }
    
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            num_str += consume();
        }
    }
    
    return {TokenType::Number, num_str};
}

JsonLexer::Token JsonLexer::lex_keyword() {
    std::string keyword;
    while (pos_ < input_.size() && std::isalpha(input_[pos_])) {
        keyword += consume();
    }
    
    if (keyword == "true"){ 
        return {TokenType::True, keyword};
    }
    
    if (keyword == "false"){ 
        return {TokenType::False, keyword};
    }
    
    if (keyword == "null"){ 
        return {TokenType::Null, keyword};
    }
    
    throw std::runtime_error("Invalid keyword: " + keyword);
}