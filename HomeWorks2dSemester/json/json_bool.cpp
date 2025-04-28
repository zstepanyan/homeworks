#include "json_bool.hpp"

JsonBoolean::JsonBoolean(bool value) : value_(value) {}

JsonBoolean::JsonBoolean(const JsonBoolean& other) : value_(other.value_) {}

JsonBoolean::JsonBoolean(JsonBoolean&& other) noexcept : value_(other.value_) {
    other.value_ = false;
}

JsonBoolean& JsonBoolean::operator=(const JsonBoolean& other) {
    if (this != &other) {
        value_ = other.value_;
    }

    return *this;
}

JsonBoolean& JsonBoolean::operator=(JsonBoolean&& other) noexcept {
    if (this != &other) {
        value_ = other.value_;
        other.value_ = false;
    }

    return *this;
}

JsonType JsonBoolean::type() const { 
    return JsonType::Boolean; 
}

std::string JsonBoolean::dump(int indent, int current_indent) const {
    return indent >= 0 ? std::string(current_indent, ' ') + (value_ ? "true" : "false") : (value_ ? "true" : "false");
}

std::unique_ptr<JsonValue> JsonBoolean::clone() const {
    return std::make_unique<JsonBoolean>(value_);
}

bool JsonBoolean::get_value() const { 
    return value_; 
}