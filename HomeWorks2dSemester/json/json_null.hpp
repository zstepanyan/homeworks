#ifndef JSON_NULL_HPP
#define JSON_NULL_HPP

#include "json_value.hpp"

class JsonNull : public JsonValue {
public:
    JsonNull() = default;
    JsonNull(const JsonNull& other);
    JsonNull(JsonNull&& other) noexcept;
    JsonNull& operator=(const JsonNull& other);
    JsonNull& operator=(JsonNull&& other) noexcept;
    JsonNull& operator=(std::unique_ptr<JsonValue>&& value) override;
    
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
};

#endif // JSON_NULL_HPP