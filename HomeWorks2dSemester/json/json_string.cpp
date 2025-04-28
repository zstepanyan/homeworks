#include "json_string.hpp"
#include <sstream>

JsonString::JsonString(const std::string& value) : value_(value) {}

JsonString::JsonString(const JsonString& other) : value_(other.value_) {}

JsonString::JsonString(JsonString&& other) noexcept : value_(std::move(other.value_)) {}

JsonString& JsonString::operator=(const JsonString& other) {
    if (this != &other) {
        value_ = other.value_;
    }

    return *this;
}

JsonString& JsonString::operator=(JsonString&& other) noexcept {
    if (this != &other) {
        value_ = std::move(other.value_);
    }
    
    return *this;
}

JsonType JsonString::type() const { return JsonType::String; }

std::string JsonString::dump(int indent, int current_indent) const {
    std::ostringstream oss;
    if (indent >= 0){ 
        oss << std::string(current_indent, ' ');
    }
    
    oss << "\"" << value_ << "\"";
    return oss.str();
}

std::unique_ptr<JsonValue> JsonString::clone() const {
    return std::make_unique<JsonString>(value_);
}

const std::string& JsonString::get_value() const { 
    return value_; 
}