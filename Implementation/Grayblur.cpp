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

    //inner box
    for (uint32_t i = 1; i < height - 1; ++i){
        for(uint32_t j = 3; j < width * 3 - 3; j += 3){
            img_out[i * width * 3 + j] = gaussianblur(img_in[i * width * 3 + j + (width - 1) * 3], img_in[i * width * 3 + j + width * 3], img_in[i * width * 3 + j + (width + 1) * 3], img_in[i * width * 3 + j - 3], img_in[i * width * 3 + j], img_in[i * width * 3 + j + 3], img_in[i * width * 3 + j - (width - 1) * 3], img_in[i * width * 3 + j - width * 3], img_in[i * width * 3 + j - (width + 1) * 3]);
            img_out[i * width * 3 + j + 1] = gaussianblur(img_in[i * width * 3 + j + (width - 1) * 3 + 1], img_in[i * width * 3 + j + width * 3 + 1], img_in[i * width * 3 + j + (width + 1) * 3 + 1], img_in[i * width * 3 + j - 3 + 1], img_in[i * width * 3 + j + 1], img_in[i * width * 3 + j + 3 + 1], img_in[i * width * 3 + j - (width - 1) * 3 + 1], img_in[i * width * 3 + j - width * 3 + 1], img_in[i * width * 3 + j - (width + 1) * 3 + 1]);
            img_out[i * width * 3 + j + 2] = gaussianblur(img_in[i * width * 3 + j + (width - 1) * 3 + 2], img_in[i * width * 3 + j + width * 3 + 2], img_in[i * width * 3 + j + (width + 1) * 3 + 2], img_in[i * width * 3 + j - 3 + 2], img_in[i * width * 3 + j + 2], img_in[i * width * 3 + j + 3 + 2], img_in[i * width * 3 + j - (width - 1) * 3 + 2], img_in[i * width * 3 + j - width * 3 + 2], img_in[i * width * 3 + j - (width + 1) * 3 + 2]);
        }
    }

    //bottom
    for(uint32_t j = 3; j < width * 3 - 3; j += 3){
        img_out[j] = gaussianblur(img_in[j + (width - 1) * 3], img_in[j + width * 3], img_in[j + (width + 1) * 3], img_in[j - 3], img_in[j], img_in[j + 3], 0, 0, 0);
        img_out[j + 1] = gaussianblur(img_in[j + 1 +(width - 1) * 3], img_in[j + 1 + width * 3], img_in[j + 1 + (width + 1) * 3], img_in[j - 3 + 1], img_in[j + 1], img_in[j + 3 + 1], 0, 0, 0);
        img_out[j + 2] = gaussianblur(img_in[j + 2 +(width - 1) * 3], img_in[j + 2 + width * 3], img_in[j + 2 + (width + 1) * 3], img_in[j - 3 + 2], img_in[j + 2], img_in[j + 3 + 2], 0, 0, 0);
    }

    //top
    for(uint32_t j = 3; j < width * 3 - 3; j += 3){
        img_out[(height - 1) * width * 3 + j] = gaussianblur(0, 0, 0, img_in[(height - 1) * width * 3 + j - 3], img_in[(height - 1) * width * 3 + j], img_in[(height - 1) * width * 3 + j + 3], img_in[(height - 1) * width * 3 + j - (width - 1) * 3], img_in[(height - 1) * width * 3 + j - width * 3], img_in[(height - 1) * width * 3 + j - (width + 1) * 3]);
        img_out[(height - 1) * width * 3 + j + 1] = gaussianblur(0, 0, 0, img_in[(height - 1) * width * 3 + j - 3 + 1], img_in[(height - 1) * width * 3 + j + 1], img_in[(height - 1) * width * 3 + j + 3 + 1], img_in[(height - 1) * width * 3 + j - (width - 1) * 3 + 1], img_in[(height - 1) * width * 3 + j - width * 3 + 1], img_in[(height - 1) * width * 3 + j - (width + 1) * 3 + 1]);
        img_out[(height - 1) * width * 3 + j + 2] = gaussianblur(0, 0, 0, img_in[(height - 1) * width * 3 + j - 3 + 2], img_in[(height - 1) * width * 3 + j + 2], img_in[(height - 1) * width * 3 + j + 3 + 2], img_in[(height - 1) * width * 3 + j - (width - 1) * 3 + 2], img_in[(height - 1) * width * 3 + j - width * 3 + 2], img_in[(height - 1) * width * 3 + j - (width + 1) * 3 + 2]);
    }

    //left
    for (uint32_t i = 1; i < height - 1; ++i){
        img_out[i * width * 3] = gaussianblur(0, img_in[i * width * 3 + width * 3], img_in[i * width * 3 + (width + 1) * 3], 0, img_in[i * width * 3], img_in[i * width * 3 + 3], 0, img_in[i * width * 3 - width * 3], img_in[i * width * 3 - (width + 1) * 3]);
        img_out[i * width * 3 + 1] = gaussianblur(0, img_in[i * width * 3 + width * 3 + 1], img_in[i * width * 3 + (width + 1) * 3 + 1], 0, img_in[i * width * 3 + 1], img_in[i * width * 3 + 3 + 1], 0, img_in[i * width * 3 - width * 3 + 1], img_in[i * width * 3 - (width + 1) * 3 + 1]);
        img_out[i * width * 3 + 2] = gaussianblur(0, img_in[i * width * 3 + width * 3 + 2], img_in[i * width * 3 + (width + 1) * 3 + 2], 0, img_in[i * width * 3 + 2], img_in[i * width * 3 + 3 + 2], 0, img_in[i * width * 3 - width * 3 + 2], img_in[i * width * 3 - (width + 1) * 3 + 2]);
    } 

    //right
    for (uint32_t i = 1; i < height - 1; ++i){
        img_out[i * width * 3 + (width - 1) * 3] = gaussianblur(img_in[i * width * 3 + (width - 1) * 3 + (width - 1) * 3], img_in[i * width * 3 + (width - 1) * 3 + width * 3], 0, img_in[i * width * 3 + (width - 1) * 3 - 3], img_in[i * width * 3 + (width - 1) * 3], 0, img_in[i * width * 3 + (width - 1) * 3 - (width - 1) * 3], img_in[i * width * 3 + (width - 1) * 3 - width * 3], 0);
        img_out[i * width * 3 + (width - 1) * 3 + 1] = gaussianblur(img_in[i * width * 3 + (width - 1) * 3 + (width - 1) * 3 + 1], img_in[i * width * 3 + (width - 1) * 3 + width * 3 + 1], 0, img_in[i * width * 3 + (width - 1) * 3 - 3 + 1], img_in[i * width * 3 + (width - 1) * 3 + 1], 0, img_in[i * width * 3 + (width - 1) * 3 - (width - 1) * 3 + 1], img_in[i * width * 3 + (width - 1) * 3 - width * 3 + 1], 0);
        img_out[i * width * 3 + (width - 1) * 3 + 2] = gaussianblur(img_in[i * width * 3 + (width - 1) * 3 + (width - 1) * 3 + 2], img_in[i * width * 3 + (width - 1) * 3 + width * 3 + 2], 0, img_in[i * width * 3 + (width - 1) * 3 - 3 + 2], img_in[i * width * 3 + (width - 1) * 3 + 2], 0, img_in[i * width * 3 + (width - 1) * 3 - (width - 1) * 3 + 2], img_in[i * width * 3 + (width - 1) * 3 - width * 3 + 2], 0);
    } 

    //bottom left    
    img_out[0] = gaussianblur(0, img_in[width * 3], img_in[(width + 1) * 3], 0, img_in[0], img_in[3], 0, 0, 0);
    img_out[1] = gaussianblur(0, img_in[width * 3 + 1], img_in[(width + 1) * 3 + 1], 0, img_in[0 + 1], img_in[3 + 1], 0, 0, 0);
    img_out[2] = gaussianblur(0, img_in[width * 3 + 2], img_in[(width + 1) * 3 + 2], 0, img_in[0 + 2], img_in[3 + 2], 0, 0, 0);

    //bottom right
    img_out[width * 3 - 3] = gaussianblur(img_in[width * 3 - 3 + (width - 1) * 3], img_in[width * 3 - 3 + width * 3], 0, img_in[width * 3 - 3 - 3], img_in[width * 3 - 3], 0, 0, 0, 0);
    img_out[width * 3 - 2] = gaussianblur(img_in[width * 3 - 3 + (width - 1) * 3 + 1], img_in[width * 3 - 3 + width * 3 + 1], 0, img_in[width * 3 - 3 - 3 + 1], img_in[width * 3 - 3 + 1], 0, 0, 0, 0);
    img_out[width * 3 - 1] = gaussianblur(img_in[width * 3 - 3 + (width - 1) * 3 + 2], img_in[width * 3 - 3 + width * 3 + 2], 0, img_in[width * 3 - 3 - 3 + 2], img_in[width * 3 - 3 + 2], 0, 0, 0, 0);

    //top left
    img_out[width * 3 * (height - 1)] = gaussianblur(0, 0, 0, 0, img_in[width * 3 * (height - 1)], img_in[width * 3 * (height - 1) + 3], 0, img_in[width * 3 * (height - 1) - width * 3], img_in[width * 3 * (height - 1) - (width + 1) * 3]);
    img_out[width * 3 * (height - 1) + 1] = gaussianblur(0, 0, 0, 0, img_in[width * 3 * (height - 1) + 1], img_in[width * 3 * (height - 1) + 3 + 1], 0, img_in[width * 3 * (height - 1) - width * 3 + 1], img_in[width * 3 * (height - 1) - (width + 1) * 3 + 1]);
    img_out[width * 3 * (height - 1) + 2] = gaussianblur(0, 0, 0, 0, img_in[width * 3 * (height - 1) + 2], img_in[width * 3 * (height - 1) + 3 + 2], 0, img_in[width * 3 * (height - 1) - width * 3 + 2], img_in[width * 3 * (height - 1) - (width + 1) * 3 + 2]);

    //top right
    img_out[width * 3 * height - 3] = gaussianblur(0, 0, 0, img_in[width * 3 * height - 3 - 3], img_in[width * 3 * height - 3], 0, img_in[width * 3 * height - 3 - (width - 1) * 3], img_in[width * 3 * height - 3 - width * 3], 0);
    img_out[width * 3 * height - 2] = gaussianblur(0, 0, 0, img_in[width * 3 * height - 2 - 3], img_in[width * 3 * height - 2], 0, img_in[width * 3 * height - 2 - (width - 1) * 3], img_in[width * 3 * height - 2 - width * 3], 0);
    img_out[width * 3 * height - 1] = gaussianblur(0, 0, 0, img_in[width * 3 * height - 1 - 3], img_in[width * 3 * height - 1], 0, img_in[width * 3 * height - 1 - (width - 1) * 3], img_in[width * 3 * height - 1 - width * 3], 0);
}


uint8_t gaussianblur(uint8_t topleft, uint8_t topmid, uint8_t topright, uint8_t left, uint8_t mid, uint8_t right, uint8_t bottomleft, uint8_t bottommid, uint8_t bottomright){
    return (topleft >> 4)  + (topmid >> 3) + (topright >> 4) + (left >> 3) + (mid >> 2) + (right >> 3) + (bottomleft >> 4) + (bottommid >> 3) + (bottomright >> 4);
}