#include "Grayblur.h"
#include "iostream"


void grayblur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

}

void gray(const uint8_t* img_in, size_t length, uint8_t* img_out){
    
    uint8_t avg;
    float a = 0.3;
    float b = 0.59;
    float c = 0.11;

    for (uint32_t i = 0; i < length * 3; i += 3)
    {
        avg = (img_in[i+2] *  a + img_in[i+1] * b + img_in[i] * c);
        img_out[i + 0] = avg;
        img_out[i + 1] = avg;
        img_out[i + 2] = avg;
        
    }
    
}