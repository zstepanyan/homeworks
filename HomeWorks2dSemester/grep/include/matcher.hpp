#ifndef MATCHERS_HPP
#define MATCHERS_HPP

#include "grep_error.hpp"
#include <regex>
#include <string>
#include <iostream>

class matcher {
public:
    matcher(const std::string& pattern, bool case_insensitive, bool whole_word, bool whole_line, bool invert_match);
    bool matches(const std::string& line) const;
    std::string highlight(const std::string& line) const;

private:
    std::regex regex_;
    bool invert_match_;
};

#endif // MATCHERS_HPP