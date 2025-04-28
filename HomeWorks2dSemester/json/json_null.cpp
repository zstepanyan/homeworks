#include "json_null.hpp"

JsonNull::JsonNull(const JsonNull& /*other*/) {
    // No state to copy
}

JsonNull::JsonNull(JsonNull&& /*other*/) noexcept {
    // No state to move
}

JsonNull& JsonNull::operator=(const JsonNull& /*other*/) {
    // No state to copy
    return *this;
}

JsonNull& JsonNull::operator=(JsonNull&& /*other*/) noexcept {
    // No state to move
    return *this;
}

JsonNull& JsonNull::operator=(std::unique_ptr<JsonValue>&& value) {
    if (!value || value->type() != JsonType::Null) {
        throw std::runtime_error("Cannot assign non-null value to JsonNull");
    }
    return *this;
}

JsonType JsonNull::type() const { return JsonType::Null; }

std::string JsonNull::dump(int indent, int current_indent) const {
    return indent >= 0 ? std::string(current_indent, ' ') + "null" : "null";
}

std::unique_ptr<JsonValue> JsonNull::clone() const {
    return std::make_unique<JsonNull>();
}