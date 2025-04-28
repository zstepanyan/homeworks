#include "json_lexer.hpp"
#include "json_parser.hpp"
#include "json_value.hpp"
#include "json_object.hpp"
#include "json_array.hpp"
#include "json_string.hpp"
#include "json_number.hpp"
#include "json_bool.hpp"
#include "json_null.hpp"
#include <iostream>
#include <fstream>

// Simple test framework
class TestSuite {
public:
    void add_test(const std::string& name, bool (*test_func)()) {
        tests.push_back({name, test_func});
    }

    void run_tests() {
        size_t passed = 0;
        std::cout << "Running " << tests.size() << " tests...\n";
        for (const auto& test : tests) {
            std::cout << "Test: " << test.name << " ... ";
            try {
                bool result = test.func();
                if (result) {
                    std::cout << "PASSED\n";
                    ++passed;
                } else {
                    std::cout << "FAILED\n";
                }
            } catch (const std::exception& e) {
                std::cout << "FAILED (Exception: " << e.what() << ")\n";
            }
        }
        std::cout << "Summary: " << passed << "/" << tests.size() << " tests passed.\n";
    }

private:
    struct TestCase {
        std::string name;
        bool (*func)();
    };
    std::vector<TestCase> tests;
};

// Helper function to compare doubles
bool doubles_equal(double a, double b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}

// JsonNull test cases
bool test_jsonnull_create_dump() {
    JsonNull j;
    return j.is_null() && j.dump() == "null" && j.dump(4) == "    null";
}

bool test_jsonnull_clone() {
    JsonNull j;
    auto clone = j.clone();
    return clone->is_null() && clone->dump() == "null";
}

bool test_jsonnull_constructors_assignments() {
    JsonNull j1;
    JsonNull j2(j1); // Copy constructor
    JsonNull j3(std::move(j1)); // Move constructor
    JsonNull j4;
    j4 = j2; // Copy assignment
    JsonNull j5;
    j5 = std::move(j3); // Move assignment
    return j2.is_null() && j4.is_null() && j5.is_null();
}

// JsonBoolean test cases
bool test_jsonboolean_create_dump() {
    JsonBoolean j1(true), j2(false);
    return j1.is_boolean() && j1.get_value() && j1.dump() == "true" &&
           j2.is_boolean() && !j2.get_value() && j2.dump() == "false";
}

bool test_jsonboolean_clone() {
    JsonBoolean j(true);
    auto clone = j.clone();
    return clone->is_boolean() && dynamic_cast<JsonBoolean&>(*clone).get_value();
}

bool test_jsonboolean_constructors_assignments() {
    JsonBoolean j1(true);
    JsonBoolean j2(j1); // Copy constructor
    JsonBoolean j3(std::move(j1)); // Move constructor
    JsonBoolean j4(false);
    j4 = j2; // Copy assignment
    JsonBoolean j5(false);
    j5 = std::move(j3); // Move assignment
    return j2.get_value() && j4.get_value() && j5.get_value();
}

// JsonNumber test cases
bool test_jsonnumber_create_dump() {
    JsonNumber j1(42.0), j2(25.8);
    return j1.is_number() && doubles_equal(j1.get_value(), 42.0) && j1.dump() == "42" &&
           j2.is_number() && doubles_equal(j2.get_value(), 25.8) && j2.dump() == "25.8";
}

bool test_jsonnumber_clone() {
    JsonNumber j(25.8);
    auto clone = j.clone();
    return clone->is_number() && doubles_equal(dynamic_cast<JsonNumber&>(*clone).get_value(), 25.8);
}

bool test_jsonnumber_constructors_assignments() {
    JsonNumber j1(25.8);
    JsonNumber j2(j1); // Copy constructor
    JsonNumber j3(std::move(j1)); // Move constructor
    JsonNumber j4(0.0);
    j4 = j2; // Copy assignment
    JsonNumber j5(0.0);
    j5 = std::move(j3); // Move assignment
    return doubles_equal(j2.get_value(), 25.8) &&
           doubles_equal(j4.get_value(), 25.8) &&
           doubles_equal(j5.get_value(), 25.8);
}

// JsonString test cases
bool test_jsonstring_create_dump() {
    JsonString j("hello");
    return j.is_string() && j.get_value() == "hello" && j.dump() == "\"hello\"";
}

bool test_jsonstring_clone() {
    JsonString j("test");
    auto clone = j.clone();
    return clone->is_string() && dynamic_cast<JsonString&>(*clone).get_value() == "test";
}

bool test_jsonstring_constructors_assignments() {
    JsonString j1("test");
    JsonString j2(j1); // Copy constructor
    JsonString j3(std::move(j1)); // Move constructor
    JsonString j4("");
    j4 = j2; // Copy assignment
    JsonString j5("");
    j5 = std::move(j3); // Move assignment
    return j2.get_value() == "test" && j4.get_value() == "test" && j5.get_value() == "test";
}

// JsonArray test cases
bool test_jsonarray_create_dump() {
    JsonArray j;
    j.add_value(std::make_unique<JsonNumber>(1));
    j.add_value(std::make_unique<JsonString>("two"));
    std::string expected = "[1,\"two\"]";
    std::string expected_pretty = "[\n    1,\n    \"two\"\n]";
    return j.is_array() && j.get_values().size() == 2 &&
           j.dump() == expected && j.dump(4) == expected_pretty;
}

bool test_jsonarray_clone() {
    JsonArray j;
    j.add_value(std::make_unique<JsonNumber>(1));
    auto clone = j.clone();
    return clone->is_array() && dynamic_cast<JsonArray&>(*clone).get_values().size() == 1 &&
           dynamic_cast<JsonArray&>(*clone).get_values()[0]->is_number();
}

bool test_jsonarray_constructors_assignments() {
    JsonArray j1;
    j1.add_value(std::make_unique<JsonNumber>(1));
    JsonArray j2(j1); // Copy constructor
    JsonArray j3(std::move(j1)); // Move constructor
    JsonArray j4;
    j4 = j2; // Copy assignment
    JsonArray j5;
    j5 = std::move(j3); // Move assignment
    return j2.get_values().size() == 1 && j2[0].is_number() &&
           j4.get_values().size() == 1 && j4[0].is_number() &&
           j5.get_values().size() == 1 && j5[0].is_number();
}

bool test_jsonarray_operator_bracket() {
    JsonArray j;
    j.add_value(std::make_unique<JsonNumber>(1));
    j.add_value(std::make_unique<JsonString>("two"));
    j[0] = std::make_unique<JsonNumber>(42); // Mutable access
    const JsonArray& j_const = j;
    return j[0].is_number() && doubles_equal(dynamic_cast<const JsonNumber&>(j[0]).get_value(), 42) &&
           j_const[1].is_string() && dynamic_cast<const JsonString&>(j_const[1]).get_value() == "two";
}

// JsonObject test cases
bool test_jsonobject_create_dump() {
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    std::string expected = "{\"age\":25.8}";
    std::string expected_pretty = "{\n    \"age\": 25.8\n}";
    return j.is_object() && j.get_values().size() == 1 &&
           j.dump() == expected && j.dump(4) == expected_pretty;
}

bool test_jsonobject_clone() {
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    auto clone = j.clone();
    return clone->is_object() && dynamic_cast<JsonObject&>(*clone).get_values().size() == 1 &&
           dynamic_cast<JsonObject&>(*clone).get_values().at("age")->is_number();
}

bool test_jsonobject_has_remove() {
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    bool has_age = j.has_key("age");
    j.remove_key("age");
    bool has_age_after = j.has_key("age");
    return has_age && !has_age_after;
}

bool test_jsonobject_constructors_assignments() {
    JsonObject j1;
    j1.set_value("age", std::make_unique<JsonNumber>(25.8));
    JsonObject j2(j1); // Copy constructor
    JsonObject j3(std::move(j1)); // Move constructor
    JsonObject j4;
    j4 = j2; // Copy assignment
    JsonObject j5;
    j5 = std::move(j3); // Move assignment
    return j2.get_values().size() == 1 && j2["age"].is_number() &&
           j4.get_values().size() == 1 && j4["age"].is_number() &&
           j5.get_values().size() == 1 && j5["age"].is_number();
}

bool test_jsonobject_operator_bracket() {
    JsonObject j;
    j["age"] = std::make_unique<JsonNumber>(25.8); // Mutable access
    j["name"] = std::make_unique<JsonString>("John");
    const JsonObject& j_const = j;
    return j["age"].is_number() && doubles_equal(dynamic_cast<const JsonNumber&>(j["age"]).get_value(), 25.8) &&
           j_const["name"].is_string() && dynamic_cast<const JsonString&>(j_const["name"]).get_value() == "John";
}

// JsonLexer test cases
bool test_jsonlexer_tokens() {
    JsonLexer lexer("{\"age\": 25.8, \"name\": \"John\"}");
    std::vector<JsonLexer::Token> tokens;
    while (true) {
        auto token = lexer.next_token();
        tokens.push_back(token);
        if (token.type == JsonLexer::TokenType::End) break;
    }
    return tokens.size() == 10 &&
           tokens[0].type == JsonLexer::TokenType::LBrace &&
           tokens[1].type == JsonLexer::TokenType::String && tokens[1].value == "age" &&
           tokens[2].type == JsonLexer::TokenType::Colon &&
           tokens[3].type == JsonLexer::TokenType::Number && tokens[3].value == "25.8" &&
           tokens[4].type == JsonLexer::TokenType::Comma &&
           tokens[5].type == JsonLexer::TokenType::String && tokens[5].value == "name" &&
           tokens[6].type == JsonLexer::TokenType::Colon &&
           tokens[7].type == JsonLexer::TokenType::String && tokens[7].value == "John" &&
           tokens[8].type == JsonLexer::TokenType::RBrace &&
           tokens[9].type == JsonLexer::TokenType::End;
}

// JsonParser test cases
bool test_jsonparser_parse() {
    JsonParser parser("{\"age\": 25.8, \"name\": \"John\"}");
    auto j = parser.parse();
    if (!j->is_object()) return false;
    const auto& obj = dynamic_cast<JsonObject&>(*j).get_values();
    return obj.size() == 2 &&
           obj.at("age")->is_number() &&
           doubles_equal(dynamic_cast<JsonNumber&>(*obj.at("age")).get_value(), 25.8) &&
           obj.at("name")->is_string() &&
           dynamic_cast<JsonString&>(*obj.at("name")).get_value() == "John";
}

// JsonApi test cases
bool test_jsonapi_parse() {
    auto j = JsonApi::parse("{\"age\": 25.8, \"name\": \"John\"}");
    if (!j->is_object()) return false;
    const auto& obj = dynamic_cast<JsonObject&>(*j).get_values();
    return obj.size() == 2 &&
           obj.at("age")->is_number() &&
           doubles_equal(dynamic_cast<JsonNumber&>(*obj.at("age")).get_value(), 25.8) &&
           obj.at("name")->is_string() &&
           dynamic_cast<JsonString&>(*obj.at("name")).get_value() == "John";
}

bool test_jsonapi_read_write() {
    JsonApi api("test.json");
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    api.write(j, 4);
    
    auto j2 = api.read();
    if (!j2->is_object()) return false;
    const auto& obj = dynamic_cast<JsonObject&>(*j2).get_values();
    return obj.size() == 1 &&
           obj.at("age")->is_number() &&
           doubles_equal(dynamic_cast<JsonNumber&>(*obj.at("age")).get_value(), 25.8);
}

bool test_jsonapi_get_set() {
    JsonApi api("test.json");
    JsonObject j;
    api.set(j, "age", 25.8);
    api.set(j, "name", std::string("John"));
    api.set(j, "active", true);
    
    return doubles_equal(api.get<double>(j, "age"), 25.8) &&
           api.get<std::string>(j, "name") == "John" &&
           api.get<bool>(j, "active");
}

bool test_jsonapi_remove_has() {
    JsonApi api("test.json");
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    j.set_value("name", std::make_unique<JsonString>("John"));
    
    bool has_age = api.has(j, "age");
    api.remove(j, "age");
    bool has_age_after = api.has(j, "age");
    bool has_name = api.has(j, "name");
    
    return has_age && !has_age_after && has_name;
}

bool test_jsonapi_error_handling() {
    JsonApi api("test.json");
    JsonObject j;
    j.set_value("age", std::make_unique<JsonNumber>(25.8));
    
    bool caught = false;
    try {
        api.get<double>(j, "missing");
    } catch (const std::runtime_error&) {
        caught = true;
    }
    
    JsonNumber not_obj(42);
    bool caught2 = false;
    try {
        api.get<double>(not_obj, "key");
    } catch (const std::runtime_error&) {
        caught2 = true;
    }
    
    return caught && caught2;
}

// Main function
int main() {
    TestSuite suite;
    
    // JsonNull tests
    suite.add_test("JsonNull Create/Dump", test_jsonnull_create_dump);
    suite.add_test("JsonNull Clone", test_jsonnull_clone);
    suite.add_test("JsonNull Constructors/Assignments", test_jsonnull_constructors_assignments);
    
    // JsonBoolean tests
    suite.add_test("JsonBoolean Create/Dump", test_jsonboolean_create_dump);
    suite.add_test("JsonBoolean Clone", test_jsonboolean_clone);
    suite.add_test("JsonBoolean Constructors/Assignments", test_jsonboolean_constructors_assignments);
    
    // JsonNumber tests
    suite.add_test("JsonNumber Create/Dump", test_jsonnumber_create_dump);
    suite.add_test("JsonNumber Clone", test_jsonnumber_clone);
    suite.add_test("JsonNumber Constructors/Assignments", test_jsonnumber_constructors_assignments);
    
    // JsonString tests
    suite.add_test("JsonString Create/Dump", test_jsonstring_create_dump);
    suite.add_test("JsonString Clone", test_jsonstring_clone);
    suite.add_test("JsonString Constructors/Assignments", test_jsonstring_constructors_assignments);
    
    // JsonArray tests
    suite.add_test("JsonArray Create/Dump", test_jsonarray_create_dump);
    suite.add_test("JsonArray Clone", test_jsonarray_clone);
    suite.add_test("JsonArray Constructors/Assignments", test_jsonarray_constructors_assignments);
    suite.add_test("JsonArray Operator[]", test_jsonarray_operator_bracket);
    
    // JsonObject tests
    suite.add_test("JsonObject Create/Dump", test_jsonobject_create_dump);
    suite.add_test("JsonObject Clone", test_jsonobject_clone);
    suite.add_test("JsonObject Has/Remove", test_jsonobject_has_remove);
    suite.add_test("JsonObject Constructors/Assignments", test_jsonobject_constructors_assignments);
    suite.add_test("JsonObject Operator[]", test_jsonobject_operator_bracket);
    
    // JsonLexer tests
    suite.add_test("JsonLexer Tokens", test_jsonlexer_tokens);
    
    // JsonParser tests
    suite.add_test("JsonParser Parse", test_jsonparser_parse);
    
    // JsonApi tests
    suite.add_test("JsonApi Parse", test_jsonapi_parse);
    suite.add_test("JsonApi Read/Write", test_jsonapi_read_write);
    suite.add_test("JsonApi Get/Set", test_jsonapi_get_set);
    suite.add_test("JsonApi Remove/Has", test_jsonapi_remove_has);
    suite.add_test("JsonApi Error Handling", test_jsonapi_error_handling);
    
    suite.run_tests();
    return 0;
}