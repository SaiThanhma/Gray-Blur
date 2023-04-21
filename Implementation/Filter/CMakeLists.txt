
add_library(filter Grayblur.cpp Grayblur.h)

target_include_directories(filter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(
    bitmap
    PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wcast-align -Wzero-as-null-pointer-constant
    -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
    -Wsign-conversion -Wfloat-conversion -Wformat=2
    -Werror=vla -Wmisleading-indentation -Wduplicated-cond
    -Wduplicated-branches -Wlogical-op -Wnull-dereference
)
