cmake_minimum_required(VERSION 3.19)
project(my_shell)

set(CMAKE_CXX_STANDARD 20)

set(ALL_FILES
        main.cpp
        Shell.h
        Shell.cpp
        Status.h
        Builtin.cpp
        Builtin.h
        CustomException.cpp
        CustomException.h
        ErrorHandler.cpp
        ErrorHandler.hpp
        )

add_executable(
        my_shell
        ${ALL_FILES}
)
