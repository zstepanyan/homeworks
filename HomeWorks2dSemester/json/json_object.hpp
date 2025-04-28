#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP

#include "json_value.hpp"
#include "json_null.hpp"
#include <unordered_map>

class JsonObject : public JsonValue {
public:
    using Object = std::unordered_map<std::string, std::unique_ptr<JsonValue>>;
    
    JsonObject();
    JsonObject(const Object& values);
    JsonObject(const JsonObject& other);
    JsonObject(JsonObject&& other) noexcept;
    JsonObject& operator=(const JsonObject& other);
    JsonObject& operator=(JsonObject&& other) noexcept;
    
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
    const Object& get_values() const;
    void set_value(const std::string& key, std::unique_ptr<JsonValue> value);
    bool has_key(const std::string& key) const;
    void remove_key(const std::string& key);
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;

private:
    Object values_;
};

#endif // JSON_OBJECT_HPP