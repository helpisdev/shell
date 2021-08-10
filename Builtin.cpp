//
// Created by ilias on 9/8/21.
//

#include "Builtin.h"
#include "CustomException.h"
#include "Shell.h"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <unistd.h>

Builtin::Builtin(Shell* shell)
    : shell_(shell) {}

Status Builtin::changeDirectory(const std::vector<std::string>& arguments) const
{
    auto path_index = static_cast<size_t>(1);
    const std::string path = arguments.size() == path_index ? shell_->getHomeDirectory() : arguments.at(path_index);
    if (int error_code = chdir(path.c_str()); error_code != 0) {
        const std::string error_message =
            "Could not change directory — unexpected error occurred. \nError number: " + std::to_string(error_code) + "\nDescription: " + strerror(error_code);
        throw CustomException(error_message);
    }
    return Status::Continue;
}

Status Builtin::printWorkingDirectory([[maybe_unused]] const std::vector<std::string>& arguments) const
{
    std::cout << std::filesystem::current_path() << std::endl;
    return Status::Continue;
}

Status Builtin::exit([[maybe_unused]] const std::vector<std::string>& arguments) const
{
    return Status::Terminate;
}

const std::unordered_map<std::string, builtinFunctionPointer_t>& Builtin::getBuiltinCommands() const
{
    return builtin_commands_;
}
