//
// Created by ilias on 9/8/21.
//

#ifndef MY_SHELL_BUILTIN_H
#define MY_SHELL_BUILTIN_H

#include "Status.h"
#include <string>
#include <unordered_map>
#include <vector>

class Shell;

class Builtin
{
    using builtinFunctionPointer_t = Status (Builtin::*)(const std::vector<std::string>&) const;
    Shell* shell_;
    const std::unordered_map<std::string, builtinFunctionPointer_t> builtin_commands_{
        { "cd", &Builtin::changeDirectory },
        { "pwd", &Builtin::printWorkingDirectory },
        { "exit", &Builtin::exit }
    };

public:
    explicit Builtin(Shell* shell);
    [[nodiscard]] Status changeDirectory(const std::vector<std::string>& arguments) const;
    [[nodiscard]] Status printWorkingDirectory([[maybe_unused]] const std::vector<std::string>& arguments) const;
    [[nodiscard]] Status exit([[maybe_unused]] const std::vector<std::string>& arguments) const;
    const std::unordered_map<std::string, builtinFunctionPointer_t>& getBuiltinCommands() const;
};

#endif //MY_SHELL_BUILTIN_H
