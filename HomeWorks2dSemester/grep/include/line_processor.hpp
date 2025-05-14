#ifndef LINE_PROCESSOR_HPP
#define LINE_PROCESSOR_HPP

#include "matcher.hpp"
#include "output.hpp"
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <iostream>

class line_processor {
public:
    line_processor(const matcher& m, int before, int after, bool list_files, bool count_matches, bool color);
    
    void process(const std::string& filepath, const std::string& line, size_t line_number);
    void flush_file(const std::string& filepath);
    const std::map<std::string, size_t>& get_counts() const;

private:
    struct line_info {
        std::string text;
        size_t number;
        bool matched;
    };

    const matcher& matcher_;
    int context_before_;
    int context_after_;
    bool list_files_;
    bool count_matches_;
    bool color_;
    std::deque<line_info> buffer_;
    std::map<std::string, size_t> match_counts_;
    bool file_has_matches_;
};


#endif // LINE_PROCESSOR_HPP