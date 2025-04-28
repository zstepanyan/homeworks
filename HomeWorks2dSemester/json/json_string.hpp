#ifndef JSON_STRING_HPP
#define JSON_STRING_HPP

#include "json_value.hpp"
#include <string>

class JsonString : public JsonValue {
public:
    JsonString(const std::string& value);
    JsonString(const JsonString& other);
    JsonString(JsonString&& other) noexcept;
    JsonString& operator=(const JsonString& other);
    JsonString& operator=(JsonString&& other) noexcept;
    
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
    const std::string& get_value() const;

private:
    std::string value_;
};

#endif // JSON_STRING_HPP