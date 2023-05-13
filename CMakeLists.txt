cmake_minimum_required(VERSION 3.14)
project(Image_Processing)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp src/io.cpp src/bitmap.cpp)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE -Wall -Wextra -Wconversion -pedantic -Wfatal-errors)
