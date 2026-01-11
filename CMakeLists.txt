cmake_minimum_required(VERSION 3.16)

project(MyCoolProgram
        VERSION 1.0
        LANGUAGES CXX
)

# Be a modern C++ citizen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Your executable
add_executable(${PROJECT_NAME}
        src/main.cpp
)

# Warnings: because silence is how bugs grow
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
