#include "json_api.hpp"
#include "json_parser.hpp"
#include "json_number.hpp"
#include "json_string.hpp"
#include "json_bool.hpp"

JsonApi::JsonApi(const std::string& filepath) : filepath_(filepath) {}

std::unique_ptr<JsonValue> JsonApi::parse(const std::string& input) {
    JsonParser parser(input);
    return parser.parse();
}

std::unique_ptr<JsonValue> JsonApi::read() const {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath_);
    }
    
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return parse(content);
}

void JsonApi::write(const JsonValue& j, int indent) const {
    std::ofstream file(filepath_);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write to file: " + filepath_);
    }
    
    file << j.dump(indent);
}

void JsonApi::remove(JsonObject& j, const std::string& key) const {
    j.remove_key(key);
}

bool JsonApi::has(const JsonValue& j, const std::string& key) const {
    if (!j.is_object()){ 
        return false;
    }
    
    return dynamic_cast<const JsonObject&>(j).has_key(key);
}

template<>
double JsonApi::get<double>(const JsonValue& j, const std::string& key) const {
    if (!j.is_object()){ 
        throw std::runtime_error("JSON value is not an object");
    }
    
    const auto& obj = dynamic_cast<const JsonObject&>(j).get_values();
    auto it = obj.find(key);
    if (it == obj.end()){ 
        throw std::runtime_error("Key not found: " + key);
    }
    if (!it->second->is_number()){ 
        throw std::runtime_error("Value is not a number");
    }
    
    return dynamic_cast<const JsonNumber&>(*it->second).get_value();
}

template<>
std::string JsonApi::get<std::string>(const JsonValue& j, const std::string& key) const {
    if (!j.is_object()){ 
        throw std::runtime_error("JSON value is not an object");
    }
    
    const auto& obj = dynamic_cast<const JsonObject&>(j).get_values();
    auto it = obj.find(key);
    if (it == obj.end()){ 
        throw std::runtime_error("Key not found: " + key);
    }
    if (!it->second->is_string()){ 
        throw std::runtime_error("Value is not a string");
    }
    
    return dynamic_cast<const JsonString&>(*it->second).get_value();
}

template<>
bool JsonApi::get<bool>(const JsonValue& j, const std::string& key) const {
    if (!j.is_object()){ 
        throw std::runtime_error("JSON value is not an object");
    }
    
    const auto& obj = dynamic_cast<const JsonObject&>(j).get_values();
    auto it = obj.find(key);
    if (it == obj.end()){ 
        throw std::runtime_error("Key not found: " + key);
    }
    if (!it->second->is_boolean()){ 
        throw std::runtime_error("Value is not a boolean");
    }
    
    return dynamic_cast<const JsonBoolean&>(*it->second).get_value();
}

template<>
void JsonApi::set<double>(JsonObject& j, const std::string& key, const double& value) const {
    j.set_value(key, std::make_unique<JsonNumber>(value));
}

template<>
void JsonApi::set<std::string>(JsonObject& j, const std::string& key, const std::string& value) const {
    j.set_value(key, std::make_unique<JsonString>(value));
}

template<>
void JsonApi::set<bool>(JsonObject& j, const std::string& key, const bool& value) const {
    j.set_value(key, std::make_unique<JsonBoolean>(value));
}