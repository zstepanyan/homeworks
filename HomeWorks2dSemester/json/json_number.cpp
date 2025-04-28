#include "json_number.hpp"
#include <sstream>
#include <iomanip>

JsonNumber::JsonNumber(double value) : value_(value) {}

JsonNumber::JsonNumber(const JsonNumber& other) : value_(other.value_) {}

JsonNumber::JsonNumber(JsonNumber&& other) noexcept : value_(other.value_) {
    other.value_ = 0.0;
}

JsonNumber& JsonNumber::operator=(const JsonNumber& other) {
    if (this != &other) {
        value_ = other.value_;
    }

    return *this;
}

JsonNumber& JsonNumber::operator=(JsonNumber&& other) noexcept {
    if (this != &other) {
        value_ = other.value_;
        other.value_ = 0.0;
    }

    return *this;
}

JsonType JsonNumber::type() const { return JsonType::Number; }

std::string JsonNumber::dump(int indent, int current_indent) const {
    std::ostringstream oss;
    if (indent >= 0){
        oss << std::string(current_indent, ' ');
    }
    
    if (value_ == static_cast<int64_t>(value_)) {
        oss << static_cast<int64_t>(value_);
    }
    else {
        oss << std::fixed << std::setprecision(6) << value_;
    }
    
    return oss.str();
}

std::unique_ptr<JsonValue> JsonNumber::clone() const {
    return std::make_unique<JsonNumber>(value_);
}

double JsonNumber::get_value() const { 
    return value_; 
}