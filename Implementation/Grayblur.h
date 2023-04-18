#include <cstdio>
#include <cinttypes>

void grayblur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void gray(const uint8_t* img_in, size_t length, uint8_t* img_out);
void blur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);