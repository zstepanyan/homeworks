#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include <string>
#include <memory>

class JsonValue;
class JsonNull;
class JsonBoolean;
class JsonNumber;
class JsonString;
class JsonArray;
class JsonObject;

enum class JsonType {
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object
};

class JsonValue {
public:
    virtual ~JsonValue() = default;
    virtual JsonType type() const = 0;
    virtual std::string dump(int indent = -1, int current_indent = 0) const = 0;
    virtual std::unique_ptr<JsonValue> clone() const = 0;
    virtual JsonValue& operator=(std::unique_ptr<JsonValue>&& value) = 0;

    // Type checking
    bool is_null() const;
    bool is_boolean() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;
};

#endif // JSON_VALUE_HPP