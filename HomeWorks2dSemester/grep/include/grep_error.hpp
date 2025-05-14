#ifndef GREP_ERROR_HPP
#define GREP_ERROR_HPP

#include <stdexcept>
#include <string>
#include <string>
#include <stdexcept>

class grep_error : public std::exception {
public:
    enum class category {
        argument,
        file,
        regex,
        system
    };

    grep_error(category cat, const std::string& message);
    const char* what() const noexcept override;
    category get_category() const;

private:
    category category_;
    std::string message_;
};

#endif // GREP_ERROR_HPP