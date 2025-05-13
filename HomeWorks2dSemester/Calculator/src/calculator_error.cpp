#include "../include/calculator_error.hpp"

calculator_error::calculator_error(const std::string& message) : message_(message) {}

const char* calculator_error::what() const noexcept {
    return message_.c_str();
}