cmake_minimum_required(VERSION 3.14)
project(grayblur)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(Bitmap)
add_executable(main main.cpp Grayblur.cpp)

target_link_libraries(main bitmap)
target_compile_options(
    main
    PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wcast-align -Wzero-as-null-pointer-constant
    -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
    -Wsign-conversion -Wfloat-conversion -Wformat=2
    -Werror=vla -Wmisleading-indentation -Wduplicated-cond
    -Wduplicated-branches -Wlogical-op -Wnull-dereference
)