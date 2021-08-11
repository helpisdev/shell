//
// Created by ilias on 9/8/21.
//

#include "Shell.hpp"
#include "CustomException.hpp"
#include "ErrorHandler.hpp"
#include <algorithm>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

Shell::Shell() = default;

Status Shell::interpretCommand() const
{
    if (tokens_.empty()) {
        return Status::Continue;
    }

    Status status{ launchBuiltin(tokens_.at(static_cast<std::size_t>(0))) };

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
            c_string_tokens_.clear();
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
    bool exit{ false };
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
    std::string delimiter{ " " };
    std::size_t position;
    while ((position = input.find(delimiter)) != std::string::npos) {
        std::string token{ input.substr(static_cast<std::size_t>(0), position) };
        tokens_.push_back(token);
        input = input.erase(static_cast<std::size_t>(0), position + delimiter.length());
    }
    tokens_.push_back(input);

    c_string_tokens_.resize(tokens_.size() + static_cast<std::size_t>(1), nullptr);
    std::ranges::transform(tokens_.begin(), tokens_.end(), c_string_tokens_.begin(), [](std::string_view token) {
        return const_cast<char*>(token.data());
    });
}

Status Shell::launchProgram() const
{
    pid_t process_id{ fork() };
    const int fork_error{ errno };

    if (process_id == 0) {
        launchChildProcess();
    }
    else if (process_id < 0) {
        ErrorHandler::handleForkError(fork_error);
    }
    else {
        handleChildProcessExit(process_id);
    }
    return Status::Continue;
}

void Shell::launchChildProcess() const
{
    if (execvp(c_string_tokens_.at(static_cast<std::size_t>(0)), c_string_tokens_.data()) == -1) {
        const int execvp_error{ errno };
        ErrorHandler::handleExecError(execvp_error);
    }
}

void Shell::handleChildProcessExit(pid_t process_id) const
{
    int status;
    do {
        pid_t wait_process_id{ waitpid(process_id, &status, WUNTRACED) };
        const int wait_error{ errno };
        if (wait_process_id == -1) {
            ErrorHandler::handleWaitError(wait_error);
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

Status Shell::launchBuiltin(const std::string& builtin) const
{
    if (builtin_map_.contains(builtin)) {
        return (builtin_.*builtin_map_.at(builtin))(tokens_);
    }
    return Status::NotFound;
}

const std::string& Shell::getHomeDirectory() const
{
    return home_directory_;
}
