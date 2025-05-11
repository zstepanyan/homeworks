#include "FileSystem.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <functional>

namespace fs = std::filesystem;

std::vector<std::string> split(const std::string& input) {
    std::vector<std::string> tokens;
    std::string token;
    bool in_quotes = false;
    char quote_char = '\0';

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        if ((c == '"' || c == '\'') && (i == 0 || input[i-1] != '\\')) {
            if (!in_quotes) {
                in_quotes = true;
                quote_char = c;
            } 
            else if (quote_char == c) {
                in_quotes = false;
                quote_char = '\0';
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } 
            else {
                token += c;
            }
        } 
        else if (c == ' ' && !in_quotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } 
        else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    for (auto& t : tokens) {
        if (t.length() >= 2 && ((t.front() == '"' && t.back() == '"') || (t.front() == '\'' && t.back() == '\''))) {
            t = t.substr(1, t.length() - 2);
        }
    }

    return tokens;
}

void display_help() {
    std::cout << "Available commands:\n"
              << "  cd <path>          Change current directory (empty for root)\n"
              << "  touch <file>       Create or update timestamp of a file\n"
              << "  mkdir <dir>        Create a new directory\n"
              << "  rm <path>          Remove a file or directory\n"
              << "  cp <src> <dst>     Copy a file or directory\n"
              << "  mv <src> <dst>     Move or rename a file or directory\n"
              << "  cat <file>         Display file contents\n"
              << "  ln <target> <link> Create a symbolic link\n"
              << "  ll                 List directory contents\n"
              << "  pwd                Print current working directory\n"
              << "  help               Show this help message\n"
              << "  exit               Quit the program\n";
}

bool process_command(FileSystem& fs, const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return true;
    }

    const std::string& cmd = tokens[0];
    std::map<std::string, std::function<bool(const std::vector<std::string>&)>> commands = {
        {"cd", [&](const std::vector<std::string>& args) {
            if (args.size() > 2) {
                std::cerr << "Error: cd expects 0 or 1 argument\n";
                return false;
            }
            return fs.cd(args.size() == 1 ? "" : args[1]);
        }},
        {"touch", [&](const std::vector<std::string>& args) {
            if (args.size() != 2) {
                std::cerr << "Error: touch expects 1 argument\n";
                return false;
            }
            return fs.touch(args[1]);
        }},
        {"mkdir", [&](const std::vector<std::string>& args) {
            if (args.size() != 2) {
                std::cerr << "Error: mkdir expects 1 argument\n";
                return false;
            }
            return fs.mkdir(args[1]);
        }},
        {"rm", [&](const std::vector<std::string>& args) {
            if (args.size() != 2) {
                std::cerr << "Error: rm expects 1 argument\n";
                return false;
            }
            return fs.rm(args[1]);
        }},
        {"cp", [&](const std::vector<std::string>& args) {
            if (args.size() != 3) {
                std::cerr << "Error: cp expects 2 arguments\n";
                return false;
            }
            return fs.cp(args[1], args[2]);
        }},
        {"mv", [&](const std::vector<std::string>& args) {
            if (args.size() != 3) {
                std::cerr << "Error: mv expects 2 arguments\n";
                return false;
            }
            return fs.mv(args[1], args[2]);
        }},
        {"cat", [&](const std::vector<std::string>& args) {
            if (args.size() != 2) {
                std::cerr << "Error: cat expects 1 argument\n";
                return false;
            }
            return fs.cat(args[1]);
        }},
        {"ln", [&](const std::vector<std::string>& args) {
            if (args.size() != 3) {
                std::cerr << "Error: ln expects 2 arguments\n";
                return false;
            }
            return fs.ln(args[1], args[2]);
        }},
        {"ll", [&](const std::vector<std::string>& args) {
            if (args.size() != 1) {
                std::cerr << "Error: ll expects no arguments\n";
                return false;
            }
            fs.ll();
            return true;
        }},
        {"pwd", [&](const std::vector<std::string>& args) {
            if (args.size() != 1) {
                std::cerr << "Error: pwd expects no arguments\n";
                return false;
            }
            fs.pwd();
            return true;
        }},
        {"help", [&](const std::vector<std::string>& args) {
            if (args.size() != 1) {
                std::cerr << "Error: help expects no arguments\n";
                return false;
            }
            display_help();
            return true;
        }},
        {"exit", [&](const std::vector<std::string>& args) {
            if (args.size() != 1) {
                std::cerr << "Error: exit expects no arguments\n";
                return false;
            }
            return false;
        }}
    };

    auto it = commands.find(cmd);
    if (it == commands.end()) {
        std::cerr << "Error: Unknown command '" << cmd << "'. Type 'help' for available commands.\n";
        return true;
    }

    return it->second(tokens);
}

int main(int argc, char* argv[]) {
    FileSystem fs;

    if (argc > 1) {
        std::vector<std::string> tokens;
        for (int i = 1; i < argc; ++i) {
            tokens.emplace_back(argv[i]);
        }
        if (!process_command(fs, tokens)) {
            return 1;
        }
        return 0;
    }

    std::string input;
    while (true) {
        std::cout << fs::path(fs.pwd()).string() << "> ";
        std::getline(std::cin, input);
        if (std::cin.eof() || input.empty()) {
            continue;
        }

        std::vector<std::string> tokens = split(input);
        if (!process_command(fs, tokens)) {
            break;
        }
    }

    return 0;
}
