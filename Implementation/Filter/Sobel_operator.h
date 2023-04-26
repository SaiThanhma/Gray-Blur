#include <cstdio>
#include <cinttypes>

void sobeloperator(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void sobelgxgray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);
void sobelgygray (const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);

float gx(uint8_t topleft, uint8_t topright, uint8_t left, uint8_t right, uint8_t bottomleft, uint8_t bottomright);
float gy(uint8_t topleft, uint8_t topright, uint8_t left, uint8_t right, uint8_t bottomleft, uint8_t bottomright);