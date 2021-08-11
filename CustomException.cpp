//
// Created by ilias on 9/8/21.
//

#include "CustomException.hpp"
#include <utility>

CustomException::CustomException(std::string error_message, Status status)
    : error_message_(std::move(error_message)), status_(status) {}

const char* CustomException::what() const noexcept
{
    return this->error_message_.c_str();
}
Status CustomException::getStatus() const
{
    return status_;
}
