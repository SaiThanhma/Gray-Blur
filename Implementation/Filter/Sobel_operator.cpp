#include "sobel_operator.h"

void sobeloperator(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){
    
}

void sobelgxgray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){
    float gxvalue;
    for (int y = 1; y < height - 1; ++y){
        for(int x = 3; x < width * 3 - 3; x += 3){
            gxvalue = gx(
                img_in[y * width * 3 + x + (width - 1) * 3],  
                img_in[y * width * 3 + x + (width + 1) * 3], 
                img_in[y * width * 3 + x - 3], 
                img_in[y * width * 3 + x + 3], 
                img_in[y * width * 3 + x - (width - 1) * 3], 
                img_in[y * width * 3 + x - (width + 1) * 3]
                );
            img_out[y * width * 3 + x] = gxvalue;
            img_out[y * width * 3 + x + 1] = gxvalue;
            img_out[y * width * 3 + x + 2] = gxvalue;
        }
    }
}

void sobelgygray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

}

float gx(uint8_t topleft, uint8_t topright, uint8_t left, uint8_t right, uint8_t bottomleft, uint8_t bottomright){
    return topleft;
}

float gy(uint8_t topleft, uint8_t topright, uint8_t left, uint8_t right, uint8_t bottomleft, uint8_t bottomright){

}