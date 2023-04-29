#include <cstdio>
#include <cinttypes>
#include <memory>

void grayblur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void gray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void blurgray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void blurrbg(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
uint8_t gaussianblur3x3(uint8_t topleft, uint8_t topmid, uint8_t topright, uint8_t left, uint8_t mid, uint8_t right, uint8_t bottomleft, uint8_t bottommid, uint8_t bottomright);