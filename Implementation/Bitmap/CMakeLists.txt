add_library(bitmap Bitmap.h BitmapFileIO.h BitmapFileIO.cpp)

target_include_directories(bitmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
