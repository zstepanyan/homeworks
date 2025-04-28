#include "json_object.hpp"
#include <sstream>

JsonObject::JsonObject() = default;

JsonObject::JsonObject(const Object& values) {
    for (const auto& [key, val] : values) {
        set_value(key, val->clone());
    }
}

JsonObject::JsonObject(const JsonObject& other) {
    for (const auto& [key, val] : other.values_) {
        set_value(key, val->clone());
    }
}

JsonObject::JsonObject(JsonObject&& other) noexcept : values_(std::move(other.values_)) {}

JsonObject& JsonObject::operator=(const JsonObject& other) {
    if (this != &other) {
        values_.clear();
        for (const auto& [key, val] : other.values_) {
            set_value(key, val->clone());
        }
    }

    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept {
    if (this != &other) {
        values_ = std::move(other.values_);
    }
    
    return *this;
}

JsonType JsonObject::type() const { 
    return JsonType::Object; 
}

std::string JsonObject::dump(int indent, int current_indent) const {
    std::ostringstream oss;
    if (indent >= 0){ 
        oss << std::string(current_indent, ' ');
    }
    
    oss << "{";
    if (indent >= 0){ 
        oss << "\n";
    }
    
    size_t i = 0;
    for (const auto& [key, value] : values_) {
        if (indent >= 0){ 
            oss << std::string(current_indent + indent, ' ');
        }
        
        oss << "\"" << key << "\":";
        if (indent >= 0){ 
            oss << " ";
        }
        
        oss << value->dump(indent, indent >= 0 ? current_indent + indent : 0);
        if (i++ < values_.size() - 1){ 
            oss << ",";
        }
        
        if (indent >= 0){ 
            oss << "\n";
        }
    }
    if (indent >= 0){ 
        oss << std::string(current_indent, ' ');
    }
    
    oss << "}";
    return oss.str();
}

std::unique_ptr<JsonValue> JsonObject::clone() const {
    return std::make_unique<JsonObject>(values_);
}

const JsonObject::Object& JsonObject::get_values() const { 
    return values_; 
}

void JsonObject::set_value(const std::string& key, std::unique_ptr<JsonValue> value) {
    values_[key] = std::move(value);
}

bool JsonObject::has_key(const std::string& key) const {
    return values_.find(key) != values_.end();
}

void JsonObject::remove_key(const std::string& key) {
    values_.erase(key);
}

JsonValue& JsonObject::operator[](const std::string& key) {
    auto it = values_.find(key);
    if (it == values_.end()) {
        values_[key] = std::make_unique<JsonNull>();
    }

    return *values_[key];
}

const JsonValue& JsonObject::operator[](const std::string& key) const {
    auto it = values_.find(key);
    if (it == values_.end()) {
        throw std::out_of_range("Key not found: " + key);
    }
    
    return *it->second;
}