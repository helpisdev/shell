//
// Created by ilias on 9/8/21.
//

#include "CustomException.h"
#include <utility>

CustomException::CustomException(std::string error_message)
    : error_message_(std::move(error_message)) {}

const char* CustomException::what() const noexcept
{
    return this->error_message_.c_str();
}
