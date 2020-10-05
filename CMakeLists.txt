cmake_minimum_required(VERSION 3.16)
project(immortal-stack)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wfloat-equal)

add_executable(
        stack
        src/main.cpp
        src/stack.h)

add_executable(
        tests
        test/main.cpp
        test/testlib.h
        test/testlib.cpp
        test/stack_tests.cpp
        src/stack.h)