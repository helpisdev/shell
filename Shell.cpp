//
// Created by ilias on 9/8/21.
//

#include "Shell.h"
#include "CustomException.h"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

Shell::Shell() = default;

Status Shell::interpretCommand() const
{
    if (tokens_.empty()) {
        return Status::Continue;
    }

    Status status = launchBuiltin(tokens_.at(static_cast<size_t>(0)));

    if (status == Status::NotFound) {
        status = launchProgram();
    }

    return status;
}

void Shell::launch()
{
    Status status;
    do {
        try {
            tokens_.clear();
            std::cout << "$ " << std::flush;
            tokenizeInput();
            status = interpretCommand();
        }
        catch (const CustomException& e) {
            std::cout << e.what() << std::endl;
            status = e.getStatus();
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            status = Status::Terminate;
        }
    } while (status != Status::Terminate);
}

std::string Shell::readInput()
{
    std::string input;
    std::string line;
    bool exit = false;
    while (!exit && std::getline(std::cin >> std::ws, line)) {
        input += line;
        if (!input.ends_with('\\')) {
            exit = true;
        }
    }
    return input;
}

void Shell::tokenizeInput()
{
    std::string input{ readInput() };
    std::string token;
    std::string delimiter{ " " };
    size_t position;
    while ((position = input.find(delimiter)) != std::string::npos) {
        token = input.substr(static_cast<size_t>(0), position);
        tokens_.push_back(token);
        input = input.erase(static_cast<size_t>(0), position + delimiter.length());
    }
    tokens_.push_back(input);
}

Status Shell::launchProgram() const
{
    pid_t process_id;
    pid_t wait_process_id;
    int status;

    process_id = fork();
    const int error = errno;

    char** arguments = new char*[tokens_.size()];
    for (auto i = static_cast<size_t>(0); i < tokens_.size(); ++i) {
        arguments[i] = const_cast<char*>(tokens_.at(i).c_str());
    }

    if (process_id == 0) {
        if (execvp(arguments[0], arguments) == -1) {
            delete[] arguments;
            int error_code = errno;
            const std::string error_message =
                "Could not execute the program — unexpected error occurred. \nError number: " + std::to_string(error_code) + "\nDescription: " + strerror(error_code);
            throw CustomException(error_message, Status::Terminate);
        }
        delete[] arguments;
    }
    else if (process_id < 0) {
        delete[] arguments;
        const std::string error_message =
            "Could not execute fork — unexpected error occurred. \nError number: " + std::to_string(error) + "\nDescription: " + strerror(error);
        throw CustomException(error_message, Status::Terminate);
    }
    else {
        do {
            wait_process_id = waitpid(process_id, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    delete[] arguments;
    return Status::Continue;
}

const std::string& Shell::getHomeDirectory() const
{
    return home_directory_;
}

Status Shell::launchBuiltin(const std::string& builtin) const
{
    if (builtin_map_.contains(builtin)) {
        return (builtin_.*builtin_map_.at(builtin))(tokens_);
    }
    return Status::NotFound;
}
