cmake_minimum_required(VERSION 3.14)
project(grayblur)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(Bitmap)
add_executable(main main.cpp Grayblur.cpp)

target_link_libraries(main bitmap)