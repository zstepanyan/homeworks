#include "json_parser.hpp"

JsonParser::JsonParser(const std::string& input) : lexer_(input) {
    next_token();
}

std::unique_ptr<JsonValue> JsonParser::parse() {
    auto result = parse_value();
    if (current_token_.type != JsonLexer::TokenType::End) {
        throw std::runtime_error("Unexpected token after JSON value");
    }
    
    return result;
}

void JsonParser::next_token() {
    current_token_ = lexer_.next_token();
}

std::unique_ptr<JsonValue> JsonParser::parse_value() {
    switch (current_token_.type) {
        case JsonLexer::TokenType::LBrace: return parse_object();
        case JsonLexer::TokenType::LBracket: return parse_array();
        case JsonLexer::TokenType::String: return parse_string();
        case JsonLexer::TokenType::Number: return parse_number();
        case JsonLexer::TokenType::True:
        case JsonLexer::TokenType::False: return parse_boolean();
        case JsonLexer::TokenType::Null: return parse_null();
        default: throw std::runtime_error("Invalid JSON value");
    }
}

std::unique_ptr<JsonValue> JsonParser::parse_object() {
    if (current_token_.type != JsonLexer::TokenType::LBrace) {
        throw std::runtime_error("Expected '{'");
    }
    next_token();
    JsonObject::Object obj;
    if (current_token_.type != JsonLexer::TokenType::RBrace) {
        while (true) {
            if (current_token_.type != JsonLexer::TokenType::String) {
                throw std::runtime_error("Expected string key");
            }
            std::string key = current_token_.value;
            next_token();
            if (current_token_.type != JsonLexer::TokenType::Colon) {
                throw std::runtime_error("Expected ':'");
            }
            
            next_token();
            obj[key] = parse_value();
            if (current_token_.type == JsonLexer::TokenType::RBrace){ 
                break;
            }
            
            if (current_token_.type != JsonLexer::TokenType::Comma) {
                throw std::runtime_error("Expected ',' or '}'");
            }
            
            next_token();
        }
    }
    if (current_token_.type != JsonLexer::TokenType::RBrace) {
        throw std::runtime_error("Expected '}'");
    }
    
    next_token();
    return std::make_unique<JsonObject>(obj);
}

std::unique_ptr<JsonValue> JsonParser::parse_array() {
    if (current_token_.type != JsonLexer::TokenType::LBracket) {
        throw std::runtime_error("Expected '['");
    }
    
    
    next_token();
    auto arr = std::make_unique<JsonArray>();
    if (current_token_.type != JsonLexer::TokenType::RBracket) {
        while (true) {
            arr->add_value(parse_value());
            if (current_token_.type == JsonLexer::TokenType::RBracket){ 
                break;
            }
            if (current_token_.type != JsonLexer::TokenType::Comma) {
                throw std::runtime_error("Expected ',' or ']'");
            }
            
            next_token();
        }
    }
    if (current_token_.type != JsonLexer::TokenType::RBracket) {
        throw std::runtime_error("Expected ']'");
    }

    next_token();
    return arr;
}

std::unique_ptr<JsonValue> JsonParser::parse_string() {
    if (current_token_.type != JsonLexer::TokenType::String) {
        throw std::runtime_error("Expected string");
    }
    
    auto result = std::make_unique<JsonString>(current_token_.value);
    next_token();
    return result;
}

std::unique_ptr<JsonValue> JsonParser::parse_number() {
    if (current_token_.type != JsonLexer::TokenType::Number) {
        throw std::runtime_error("Expected number");
    }
    try {
        auto result = std::make_unique<JsonNumber>(std::stod(current_token_.value));
        next_token();
        return result;
    } catch (...) {
        throw std::runtime_error("Invalid number format: " + current_token_.value);
    }
}

std::unique_ptr<JsonValue> JsonParser::parse_boolean() {
    if (current_token_.type == JsonLexer::TokenType::True) {
        next_token();
        return std::make_unique<JsonBoolean>(true);
    }
    if (current_token_.type == JsonLexer::TokenType::False) {
        next_token();
        return std::make_unique<JsonBoolean>(false);
    }
    
    throw std::runtime_error("Expected boolean");
}

std::unique_ptr<JsonValue> JsonParser::parse_null() {
    if (current_token_.type != JsonLexer::TokenType::Null) {
        throw std::runtime_error("Expected null");
    }
    
    next_token();
    return std::make_unique<JsonNull>();
}