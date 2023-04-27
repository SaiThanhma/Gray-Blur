
add_library(mymath Mymath.cpp Mymath.h)

target_include_directories(mymath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
