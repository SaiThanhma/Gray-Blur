
add_library(filter sobel.h sobel.cpp convolution.h convolution.cpp gaussian.h gaussian.cpp)

target_include_directories(filter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(filter mymath)

target_compile_options(
    filter
    PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wcast-align -Wzero-as-null-pointer-constant
    -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
    -Wsign-conversion -Wfloat-conversion -Wformat=2
    -Werror=vla -Wmisleading-indentation -Wduplicated-cond
    -Wduplicated-branches -Wlogical-op -Wnull-dereference
)
