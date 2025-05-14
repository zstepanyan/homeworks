#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>
#include <iostream>
#include <chrono>

class output {
public:
    static void print(const std::string& filepath, std::size_t line_number, const std::string& line);
    static void print_filename(const std::string& filepath);
    static void print_count(const std::string& filepath, std::size_t count);
    static void print_summary(const std::string& filepath, std::size_t match_count);
};

#endif // OUTPUT_HPP