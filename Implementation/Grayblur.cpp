#include "Grayblur.h"
#include "iostream"


void grayblur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

}

void gray(const uint8_t* img_in, size_t length, uint8_t* img_out){
    
    uint8_t avg;
    double a = 0.3;
    double b = 0.59;
    double c = 0.11;

    for (uint32_t i = 0; i < length * 3; i += 3)
    {
        avg = static_cast<uint8_t>(img_in[i+2] *  a + img_in[i+1] * b + img_in[i] * c);
        img_out[i + 0] = avg;
        img_out[i + 1] = avg;
        img_out[i + 2] = avg;
    }
}

void blur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

    for (uint32_t i = 1; i < height - 1; ++i){
        for(uint32_t j = 3; j < width * 3 - 3; j += 3){
            img_out[i * width * 3 + j] = 255;
            img_out[i * width * 3 + j + 1] = 0;
            img_out[i * width * 3 + j + 2] = 0;
        }
    }

    for (uint32_t i = 0; i < height; i += height - 1){
        for(uint32_t j = 3; j < width * 3 - 3; j += 3){
            img_out[i * width * 3 + j] = 0;
            img_out[i * width * 3 + j + 1] = 0;
            img_out[i * width * 3 + j + 2] = 255;
        }
    }

    for (uint32_t i = 1; i < height - 1; ++i){
        for(uint32_t j = 0; j < width * 3; j += width * 3 - 3){
            img_out[i * width * 3 + j] = 0;
            img_out[i * width * 3 + j + 1] = 0;
            img_out[i * width * 3 + j + 2] = 255;
        }
    }

    

}