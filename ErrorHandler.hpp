//
// Created by ilias on 11/8/21.
//

#ifndef MY_SHELL_ERRORHANDLER_HPP
#define MY_SHELL_ERRORHANDLER_HPP

class ErrorHandler
{
public:
    static void handleExecError(const int& error_code);
    static void handleForkError(const int& error_code);
    static void handleWaitError(const int& error_code);
};

#endif //MY_SHELL_ERRORHANDLER_HPP
