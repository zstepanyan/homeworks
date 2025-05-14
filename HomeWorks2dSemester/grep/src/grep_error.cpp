#include "../include/grep_error.hpp"

grep_error::grep_error(category cat, const std::string& message)
    : category_(cat), message_(message) {}

const char* grep_error::what() const noexcept {
    return message_.c_str();
}

grep_error::category grep_error::get_category() const {
    return category_;
}