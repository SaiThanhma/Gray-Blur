
add_library(filter sobel.h sobel.cpp convolution.h convolution.cpp gaussian.h gaussian.cpp)

target_include_directories(filter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(filter)