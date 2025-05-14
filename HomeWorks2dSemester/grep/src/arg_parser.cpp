#include "arg_parser.hpp"

args arg_parser::parse(int argc, char* argv[]) {
    args result;
    if (argc < 2) {
        throw grep_error(grep_error::category::argument, "Usage: " + std::string(argv[0]) + " [options] pattern [file...]");
    }

    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        std::string opt = argv[i];
        if (opt == "-i") {
            result.case_insensitive = true;
        } 
        else if (opt == "-r") {
            result.recursive = true;
        } 
        else if (opt == "-w") {
            result.whole_word = true;
        } 
        else if (opt == "-E") {
            result.whole_line = true;
        } 
        else if (opt == "-v") {
            result.invert_match = true;
        } 
        else if (opt == "-l") {
            result.list_files = true;
        } 
        else if (opt == "-c") {
            result.count_matches = true;
        } 
        else if (opt == "--color") {
            result.color = true;
        } 
        else if (opt.substr(0, 2) == "-A") {
            if (i + 1 >= argc) {
                throw grep_error(grep_error::category::argument, "-A requires a number");
            }

            result.context_after = std::stoi(argv[++i]);
        } 
        else if (opt.substr(0, 2) == "-B") {
            if (i + 1 >= argc) {
                throw grep_error(grep_error::category::argument, "-B requires a number");
            }

            result.context_before = std::stoi(argv[++i]);
        } 
        else if (opt.substr(0, 2) == "-C") {
            if (i + 1 >= argc) {
                throw grep_error(grep_error::category::argument, "-C requires a number");
            }
            
            result.context_before = result.context_after = std::stoi(argv[++i]);
        } 
        else if (opt.substr(0, 9) == "--include=") {
            std::string ext = opt.substr(9);
            if (!ext.empty() && ext[0] == '*') {
                ext = ext.substr(1);
            }
            
            result.include_extensions.push_back(ext);
        } 
        else {
            throw grep_error(grep_error::category::argument, "Unknown option: " + opt);
        }
        
        ++i;
    }

    if (i >= argc) {
        throw grep_error(grep_error::category::argument, "Pattern required");
    }
    
    result.pattern = argv[i++];
    if (i >= argc) {
        result.paths.push_back(".");
    }
    else {
        for (; i < argc; ++i) {
            result.paths.push_back(argv[i]);
        }
    }

    validate(result);
    return result;
}

void arg_parser::validate(const args& a) {
    if (a.context_before < 0 || a.context_after < 0) {
        throw grep_error(grep_error::category::argument, "Context lines cannot be negative");
    }
    if (a.list_files && a.count_matches) {
        throw grep_error(grep_error::category::argument, "Cannot combine -l and -c");
    }
    if (a.whole_word && a.whole_line) {
        throw grep_error(grep_error::category::argument, "Cannot combine -w and -E");
    }
}