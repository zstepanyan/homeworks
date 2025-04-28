
#include "json_value.hpp"

class JsonNumber : public JsonValue {
public:
    JsonNumber(double value);
    JsonNumber(const JsonNumber& other);
    JsonNumber(JsonNumber&& other) noexcept;
    JsonNumber& operator=(const JsonNumber& other);
    JsonNumber& operator=(JsonNumber&& other) noexcept;
    
    JsonType type() const override;
    std::string dump(int indent = -1, int current_indent = 0) const override;
    std::unique_ptr<JsonValue> clone() const override;
    double get_value() const;

private:
    double value_;
};