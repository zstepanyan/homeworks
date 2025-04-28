#include "json_value.hpp"

bool JsonValue::is_null() const { 
    return type() == JsonType::Null; 
}

bool JsonValue::is_boolean() const { 
    return type() == JsonType::Boolean;
}

bool JsonValue::is_number() const { 
    return type() == JsonType::Number;
}

bool JsonValue::is_string() const { 
    return type() == JsonType::String;
}

bool JsonValue::is_array() const { 
    return type() == JsonType::Array; 
}

bool JsonValue::is_object() const { 
    return type() == JsonType::Object;
}