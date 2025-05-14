#include "../include/line_processor.hpp"

line_processor::line_processor(const matcher& m, int before, int after, bool list_files, bool count_matches, bool color)
    : matcher_(m), context_before_(before), context_after_(after),
      list_files_(list_files), count_matches_(count_matches), color_(color), file_has_matches_(false) {}

void line_processor::process(const std::string& filepath, const std::string& line, size_t line_number) {
    bool matched = matcher_.matches(line);
    if (matched) {
        file_has_matches_ = true;
        match_counts_[filepath]++;
    }

    if (count_matches_ || (list_files_ && !matched)) {
        return;
    }

    buffer_.push_back({line, line_number, matched});
    while (buffer_.size() > static_cast<size_t>(context_before_ + context_after_ + 1)) {
        auto& front = buffer_.front();
        if (front.matched) {
            for (const auto& li : buffer_) {
                if (li.number <= front.number + context_after_) {
                    std::string output_text = color_ ? matcher_.highlight(li.text) : li.text;
                    output::print(filepath, li.number, output_text);
                }
            }
        } 
        else if (buffer_.size() > static_cast<size_t>(context_before_)) {
            bool should_print = false;
            for (size_t i = 1; i < buffer_.size() && i <= static_cast<size_t>(context_after_); ++i) {
                if (buffer_[i].matched) {
                    should_print = true;
                    break;
                }
            }
        
            if (should_print) {
                std::string output_text = color_ ? matcher_.highlight(front.text) : front.text;
                output::print(filepath, front.number, output_text);
            }
        }
        
        buffer_.pop_front();
    }
}

void line_processor::flush_file(const std::string& filepath) {
    if (match_counts_[filepath] > 0) {
        output::print_summary(filepath, match_counts_[filepath]);
    }
    
    if (count_matches_) {
        output::print_count(filepath, match_counts_[filepath]);
    } 
    else if (list_files_ && file_has_matches_) {
        output::print_filename(filepath);
    } 
    else {
        for (const auto& li : buffer_) {
            if (li.matched || li.number <= buffer_.back().number + context_after_) {
                std::string output_text = color_ ? matcher_.highlight(li.text) : li.text;
                output::print(filepath, li.number, output_text);
            }
        }
    }
    
    buffer_.clear();
    file_has_matches_ = false;
}

const std::map<std::string, size_t>& line_processor::get_counts() const {
    return match_counts_;
}