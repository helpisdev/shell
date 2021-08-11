//
// Created by ilias on 11/8/21.
//

#include "ErrorHandler.hpp"
#include "CustomException.h"
#include "Status.h"
#include <cstring>
#include <string>

void ErrorHandler::handleExecError(const int& error_code)
{
    const std::string error_message{
        "Could not execute execvp — unexpected error occurred. \nError number: " + std::to_string(error_code) + "\nDescription: " + strerror(error_code)
    };
    throw CustomException(error_message, Status::Terminate);
}
void ErrorHandler::handleWaitError(const int& error_code)
{
    const std::string error_message{
        "Could not execute wait — unexpected error occurred. \nError number: " + std::to_string(error_code) + "\nDescription: " + strerror(error_code)
    };
    throw CustomException(error_message, Status::Terminate);
}
void ErrorHandler::handleForkError(const int& error_code)
{
    const std::string error_message{
        "Could not execute fork — unexpected error occurred. \nError number: " + std::to_string(error_code) + "\nDescription: " + strerror(error_code)
    };
    throw CustomException(error_message, Status::Terminate);
}
