//
// Created by ilias on 9/8/21.
//

#ifndef MY_SHELL_SHELL_H
#define MY_SHELL_SHELL_H

#include "Builtin.h"

using builtinFunctionPointer_t = Status (Builtin::*)(const std::vector<std::string>&) const;
using commandToFunctionMap_t = const std::unordered_map<std::string, builtinFunctionPointer_t>;

class Shell
{
private:
    const std::string home_directory_{ secure_getenv("HOME") };
    std::vector<std::string> tokens_{ std::vector<std::string>() };
    const Builtin builtin_{ Builtin(this) };
    const commandToFunctionMap_t builtin_map_ = builtin_.getBuiltinCommands();

public:
    Shell();
    void launch();
    [[nodiscard]] const std::string& getHomeDirectory() const;

private:
    static std::string readInput();
    void tokenizeInput();
    Status interpretCommand() const;
    [[nodiscard]] Status launchBuiltin(const std::string& builtin) const;
    Status launchProgram() const;
};

#endif //MY_SHELL_SHELL_H
