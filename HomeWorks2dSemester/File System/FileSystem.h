#ifndef FILE_SYSTEM
#define FILE_SYSTEM

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

namespace fs = std::filesystem;

class FileSystem {
public:
    FileSystem() : cwd(fs::current_path()) {}
    ~FileSystem() = default;

    bool cd(const std::string& path);
    bool touch(const std::string& file);
    bool mkdir(const std::string& dir);
    bool rm(const std::string& path);
    bool cp(const std::string& from, const std::string& to);
    bool mv(const std::string& from, const std::string& to);
    bool cat(const std::string& file);
    bool ln(const std::string& target, const std::string& link);
    void ll() const;
    void pwd() const;

private:
    fs::path cwd;

    bool exists_in_cwd(const fs::path& path) const {
        return fs::exists(cwd / path);
    }
};

#endif // FILE_SYSTEM