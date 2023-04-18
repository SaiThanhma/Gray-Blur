add_library(bitmap Bitmap.h BitmapFileIO.h BitmapFileIO.cpp)

target_include_directories(bitmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
