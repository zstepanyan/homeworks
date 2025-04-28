#ifndef JSON_API_HPP
#define JSON_API_HPP

#include "json_value.hpp"
#include "json_object.hpp"
#include <string>
#include <fstream>

class JsonApi {
public:
    JsonApi(const std::string& filepath);
    std::unique_ptr<JsonValue> read() const;
    void write(const JsonValue& j, int indent = 4) const;
    template<typename T>
    T get(const JsonValue& j, const std::string& key) const;
    template<typename T>
    void set(JsonObject& j, const std::string& key, const T& value) const;
    void remove(JsonObject& j, const std::string& key) const;
    bool has(const JsonValue& j, const std::string& key) const;
    static std::unique_ptr<JsonValue> parse(const std::string& input);

private:
    std::string filepath_;
};

#endif // JSON_API_HPP