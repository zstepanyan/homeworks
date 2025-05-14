#include "../include/file_reader.hpp"

void file_reader::read(const std::vector<std::string> &paths, bool recursive, const std::vector<std::string> &include_extensions, function_type line_callback)
{
    namespace fs = std::filesystem;
    std::mutex mutex;
    std::vector<std::thread> threads;
    for (const auto& path : paths) {
        fs::path p(path);
        if (!fs::exists(p)) {
            std::lock_guard<std::mutex> lock(mutex);
            std::cerr << "Warning: Path does not exist: " << path <<  std::endl;
            continue;
        }
        
        if (fs::is_directory(p) && recursive) {
            for (const auto& entry : fs::recursive_directory_iterator(p, fs::directory_options::skip_permission_denied)) {
                if (fs::is_regular_file(entry.path()) && matches_extension(entry.path().string(), include_extensions)) {
                    threads.emplace_back(process_file, fs::absolute(entry.path()).string(), line_callback, std::ref(mutex));
                }
            }
        }
        else if (fs::is_regular_file(p) && matches_extension(p.string(), include_extensions)) {
            threads.emplace_back(process_file, fs::absolute(p).string(), line_callback, std::ref(mutex));
        }
        else if (!fs::is_directory(p)) {
            std::lock_guard<std::mutex> lock(mutex);
            std::cerr << "Warning: Not a file: " << path << std::endl;
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}

void file_reader::process_file(const std::string &filepath, function_type callback, std::mutex &mutex)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::lock_guard<std::mutex> lock(mutex);
        std::cerr << "Warning cannot open file " << filepath << std::endl;
        return;
    }

    std::string line;
    std::size_t line_number = 0;
    while (std::getline(file, line)) {
        ++line_number;
        std::lock_guard<std::mutex> lock(mutex);
        callback(filepath, line, line_number);
    }

    file.close();
}

bool file_reader::matches_extension(const std::string &filepath, const std::vector<std::string> &extensions)
{
    if (extensions.empty()) {
        return true;
    }

    namespace fs = std::filesystem;
    fs::path p(filepath);
    std::string ext = p.extension().string();
    if (!ext.empty() && ext[0] == '.') {
        ext = ext.substr(1);
    }

    return std::find(extensions.begin(), extensions.end(), ext) != extensions.end();
}
