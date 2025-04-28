#include "json_array.hpp"

JsonArray::JsonArray() = default;

JsonArray::JsonArray(const Array& values) {
    for (const auto& val : values) {
        add_value(val->clone());
    }
}

JsonArray::JsonArray(const JsonArray& other) {
    for (const auto& val : other.values_) {
        add_value(val->clone());
    }
}

JsonArray::JsonArray(JsonArray&& other) noexcept : values_(std::move(other.values_)) {}

JsonArray& JsonArray::operator=(const JsonArray& other) {
    if (this != &other) {
        values_.clear();
        for (const auto& val : other.values_) {
            add_value(val->clone());
        }
    }

    return *this;
}

JsonArray& JsonArray::operator=(JsonArray&& other) noexcept {
    if (this != &other) {
        values_ = std::move(other.values_);
    }

    return *this;
}

JsonType JsonArray::type() const { 
    return JsonType::Array; 
}

std::string JsonArray::dump(int indent, int current_indent) const {
    std::ostringstream oss;
    if (indent >= 0){ 
        oss << std::string(current_indent, ' ');
    }
    
    oss << "[";
    if (indent >= 0){ 
        oss << "\n";
    }
    
    for (size_t i = 0; i < values_.size(); ++i) {
        oss << values_[i]->dump(indent, indent >= 0 ? current_indent + indent : 0);
        if (i < values_.size() - 1){ 
            oss << ",";
        }
        
        if (indent >= 0){ 
            oss << "\n";
        }
    }
    
    if (indent >= 0){ 
        oss << std::string(current_indent, ' ');
    }
    
    oss << "]";
    return oss.str();
}

std::unique_ptr<JsonValue> JsonArray::clone() const {
    return std::make_unique<JsonArray>(values_);
}

const JsonArray::Array& JsonArray::get_values() const { 
    return values_; 
}

void JsonArray::add_value(std::unique_ptr<JsonValue> value) {
    values_.push_back(std::move(value));
}

JsonValue& JsonArray::operator[](size_t index) {
    if (index >= values_.size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(index));
    }

    return *values_[index];
}

const JsonValue& JsonArray::operator[](size_t index) const {
    if (index >= values_.size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(index));
    }

    return *values_[index];
}