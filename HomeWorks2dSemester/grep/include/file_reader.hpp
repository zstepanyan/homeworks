#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "grep_error.hpp"
#include <iostream>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

class file_reader {
public:
    using function_type = std::function<void(const std::string&, const std::string&, std::size_t)>;

    static void read(const std::vector<std::string>& paths, bool recursive, const std::vector<std::string>& include_extensions,
                     function_type line_callback);
private:
    static void process_file(const std::string& filepath, function_type callback, std::mutex& mutex);
    static bool matches_extension(const std::string& filepath, const std::vector<std::string>& extensions);
};

#endif // FILE_READER_HPP