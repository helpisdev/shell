//
// Created by ilias on 9/8/21.
//

#ifndef MY_SHELL_CUSTOMEXCEPTION_HPP
#define MY_SHELL_CUSTOMEXCEPTION_HPP

#include "Status.hpp"
#include <exception>
#include <string>

class CustomException : public std::exception
{
private:
    const std::string error_message_;
    const Status status_;

public:
    explicit CustomException(std::string error_message, Status status);
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] Status getStatus() const;
};

#endif //MY_SHELL_CUSTOMEXCEPTION_HPP
