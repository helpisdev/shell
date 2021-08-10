//
// Created by ilias on 9/8/21.
//

#ifndef MY_SHELL_CUSTOMEXCEPTION_H
#define MY_SHELL_CUSTOMEXCEPTION_H

#include <exception>
#include <string>

class CustomException : public std::exception
{
private:
    const std::string error_message_;

public:
    explicit CustomException(std::string error_message);
    [[nodiscard]] const char* what() const noexcept override;
};

#endif //MY_SHELL_CUSTOMEXCEPTION_H
