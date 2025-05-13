#ifndef CALCULATOR_ERROR_HPP
#define CALCULATOR_ERROR_HPP

#include <string>
#include <exception>

class calculator_error : public std::exception {
public:
    explicit calculator_error(const std::string& message);
    const char* what() const noexcept override;

private:
    std::string message_;
};

#endif // CALCULATOR_ERROR_HPP