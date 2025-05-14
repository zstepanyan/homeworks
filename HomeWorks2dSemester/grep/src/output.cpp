#include "../include/output.hpp"
#include <chrono>
#include <iomanip>

void output::print(const std::string& filepath, size_t line_number, const std::string& line) {
    std::cout << filepath << ":" << line_number << ":" << line << std::endl;
}

void output::print_filename(const std::string& filepath) {
    std::cout << filepath << std::endl;
}

void output::print_count(const std::string& filepath, size_t count) {
    std::cout << filepath << ":" << count << std::endl;
}

void output::print_summary(const std::string& filepath, size_t match_count) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
              << " \033[1;34m" << filepath << "\033[0m " << match_count << " line(s) matched" << std::endl;
}