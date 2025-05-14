#include "../include/matcher.hpp"

matcher::matcher(const std::string &pattern, bool case_insensitive, bool whole_word, bool whole_line, bool invert_match) :
    invert_match_(invert_match)
{
    std::string final_pattern;
    if (whole_line) {
        final_pattern = "^" + pattern + "$";
    }
    else if (whole_word) {
        final_pattern = "\\b" + pattern + "\\b";
    }
    else {
        final_pattern = pattern;
    }

    std::regex::flag_type flags = std::regex::ECMAScript | std::regex::multiline;
    if (case_insensitive) {
        flags |= std::regex::icase;
    }

    try {
        regex_ = std::regex(final_pattern, flags);
    } catch (const std::regex_error& e) {
        std::string error_message = "Invalid regex pattern";
        switch (e.code()) {
            case std::regex_constants::error_collate :
                error_message += "Invalid collating element";
                break;
            case std::regex_constants::error_ctype :
                error_message += "Invalid character class";
                break;
            case std::regex_constants::error_escape :
                error_message += "Invalid escape sequence";
                break;
            case std::regex_constants::error_backref:
                error_message += "Invalid back reference";
                break;
            case std::regex_constants::error_brack:
                error_message += "Mismatched brackets";
                break;
            case std::regex_constants::error_paren:
                error_message += "Mismatched parentheses";
                break;
            case std::regex_constants::error_brace:
                error_message += "Mismatched braces";
                break;
            case std::regex_constants::error_badbrace:
                error_message += "Invalid range in braces";
                break;
            case std::regex_constants::error_range:
                error_message += "Invalid character range";
                break;
            case std::regex_constants::error_space:
                error_message += "Insufficient memory";
                break;
            case std::regex_constants::error_badrepeat:
                error_message += "Invalid repeat operator";
                break;
            case std::regex_constants::error_complexity:
                error_message += "Expression too complex";
                break;
            case std::regex_constants::error_stack:
                error_message += "Stack overflow";
                break;
            default:
                error_message += e.what();
        }
 
        throw grep_error(grep_error::category::regex, error_message);
    }
}

bool matcher::matches(const std::string &line) const
{
    bool match = std::regex_search(line, regex_);
    return invert_match_ ? !match : match;
}

std::string matcher::highlight(const std::string &line) const
{
    std::string result;
    std::sregex_iterator it(line.begin(), line.end(), regex_);
    std::sregex_iterator end;
    std::size_t pos = 0;
    while (it != end) {
        std::size_t start = it->position();
        std::size_t length = it->length();
        result += line.substr(pos, start - pos);
        result += "\033[1;31m" + it->str() + "\033[0m"; // Red color for matches
        pos = start +  length;
        ++it;
    }

    result += line.substr(pos);
    return result;
}
