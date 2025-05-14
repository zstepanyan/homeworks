#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <string>
#include <vector>
#include "grep_error.hpp"

struct args {
    std::string pattern;
    std::vector<std::string> paths;
    bool case_insensitive = false;
    bool recursive = false;
    bool whole_word = false;
    bool whole_line = false;
    bool invert_match = false;
    bool list_files = false;
    bool count_matches = false;
    int context_before = false;
    int context_after = false;
    bool color = false;
    std::vector<std::string> include_extensions;
};

class arg_parser {
public:
    static args parse(int argc, char* argv[]);

private:
    static void validate(const args& a);
};

#endif // ARG_PARSER_HPP