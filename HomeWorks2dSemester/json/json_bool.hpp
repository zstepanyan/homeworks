#ifndef JSON_BOOL_HPP
#define JSON_BOOL_HPP

#include "json_value.hpp"

class JsonBoolean : public JsonValue {
public:
    JsonBoolean(bool value);
    JsonBoolean(const JsonBoolean& other);
    JsonBoolean(JsonBoolean&& other) noexcept;
    JsonBoolean& operator=(const JsonBoolean& other);
    JsonBoolean& operator=(JsonBoolean&& other) noexcept;
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
    bool get_value() const;

private:
    bool value_;
};

#endif // JSON_BOOL_HPP