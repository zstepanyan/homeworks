#ifndef JSON_ARRAY_HPP
#define JSON_ARRAY_HPP

#include "json_value.hpp"
#include <vector>
#include <sstream>
#include <stdexcept>

class JsonArray : public JsonValue {
public:
    using Array = std::vector<std::unique_ptr<JsonValue>>;
    
    JsonArray();
    JsonArray(const Array& values);
    JsonArray(const JsonArray& other);
    JsonArray(JsonArray&& other) noexcept;
    JsonArray& operator=(const JsonArray& other);
    JsonArray& operator=(JsonArray&& other) noexcept;
    
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
    const Array& get_values() const;
    void add_value(std::unique_ptr<JsonValue> value);
    JsonValue& operator[](size_t index);
    const JsonValue& operator[](size_t index) const;

private:
    Array values_;
};

#endif // JSON_ARRAY_HPP