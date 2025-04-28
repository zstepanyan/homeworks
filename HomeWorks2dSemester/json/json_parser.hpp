#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "json_lexer.hpp"
#include "json_value.hpp"
#include "json_object.hpp"
#include "json_array.hpp"
#include "json_string.hpp"
#include "json_number.hpp"
#include "json_bool.hpp"
#include "json_null.hpp"

class JsonParser {
public:
    JsonParser(const std::string& input);
    std::unique_ptr<JsonValue> parse();

private:
    JsonLexer lexer_;
    JsonLexer::Token current_token_;
    void next_token();
    
    std::unique_ptr<JsonValue> parse_value();
    std::unique_ptr<JsonValue> parse_object();
    std::unique_ptr<JsonValue> parse_array();
    std::unique_ptr<JsonValue> parse_string();
    std::unique_ptr<JsonValue> parse_number();
    std::unique_ptr<JsonValue> parse_boolean();
    std::unique_ptr<JsonValue> parse_null();
};

#endif // JSON_PARSER_HPP