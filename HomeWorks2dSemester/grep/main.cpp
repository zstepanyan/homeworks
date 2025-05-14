#include "include/arg_parser.hpp"
#include "include/file_reader.hpp"
#include "include/matcher.hpp"
#include "include/line_processor.hpp"
#include "include/grep_error.hpp"
#include <iostream>

int main() {
    std::string line;
    while (true) {
        std::cout << ">> ";
        std::cout.flush(); 
        if (!std::getline(std::cin, line)) {
            break; //  EOF (e.g., Ctrl+D)
        }

        // empty input
        if (line.empty()) {
            continue;
        }

        // exit command
        if (line == "exit") {
            break;
        }

        // Split input 
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        // first token is "grep"
        if (tokens.empty() || tokens[0] != "grep") {
            std::cerr << "Error: Command must start with 'grep'" << std::endl;
            continue;
        }

        // tokens to argv format
        std::vector<char*> argv;
        for (auto& t : tokens) {
            argv.push_back(t.data());
        }

        argv.push_back(nullptr); // Null-terminate argv

        try {
            // argument parsing
            args arguments = arg_parser::parse(argv.size() - 1, argv.data());

            // grep command processing 
            matcher pattern_matcher(arguments.pattern, arguments.case_insensitive, arguments.whole_word,
                                   arguments.whole_line, arguments.invert_match);
            line_processor processor(pattern_matcher, arguments.context_before, arguments.context_after,
                                    arguments.list_files, arguments.count_matches, arguments.color);

            std::string current_file;
            file_reader::read(arguments.paths, arguments.recursive, arguments.include_extensions,
                [&processor, &current_file](const std::string& filepath, const std::string& line, size_t line_number) {
                    if (current_file != filepath && !current_file.empty()) {
                        processor.flush_file(current_file);
                    }
                    
                    current_file = filepath;
                    processor.process(filepath, line, line_number);
                });

            if (!current_file.empty()) {
                processor.flush_file(current_file);
            }

            if (arguments.count_matches && arguments.paths.size() > 1) {
                size_t total = 0;
                for (const auto& [file, count] : processor.get_counts()) {
                    total += count;
                }
                
                std::cout << "Total matches: " << total << std::endl;
            }
        } catch (const grep_error& e) {
            std::cerr << "Error [" << (e.get_category() == grep_error::category::argument ? "argument" :
                                       e.get_category() == grep_error::category::file ? "file" :
                                       e.get_category() == grep_error::category::regex ? "regex" : "system")
                      << "]: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error [system]: " << e.what() << std::endl;
        }
    }
    
    return 0;
}