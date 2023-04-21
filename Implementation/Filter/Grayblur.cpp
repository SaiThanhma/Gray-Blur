#include "Grayblur.h"
#include "iostream"


void grayblur(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

    std::unique_ptr<uint8_t[]> tmpgray = std::make_unique<uint8_t[]>(width * height * 3);
    gray(img_in, width, height, tmpgray.get());
    blurrbg(tmpgray.get(), width, height, img_out);

}

void gray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

    uint8_t avg;
    double a = 0.3;
    double b = 0.59;
    double c = 0.11;

    for (int i = 0; i < width * height * 3; i += 3)
    {
        avg = static_cast<uint8_t>(img_in[i+2] *  a + img_in[i+1] * b + img_in[i] * c);
        img_out[i + 0] = avg;
        img_out[i + 1] = avg;
        img_out[i + 2] = avg;
    }

}
void blurgray(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){
    
    uint8_t grayblurvalue;
    //inner box
    for (int y = 1; y < height - 1; ++y){
        for(int x = 3; x < width * 3 - 3; x += 3){
            grayblurvalue = gaussianblur3x3(
                img_in[y * width * 3 + x + (width - 1) * 3], 
                img_in[y * width * 3 + x + width * 3], 
                img_in[y * width * 3 + x + (width + 1) * 3], 
                img_in[y * width * 3 + x - 3], 
                img_in[y * width * 3 + x], 
                img_in[y * width * 3 + x + 3], 
                img_in[y * width * 3 + x - (width - 1) * 3], 
                img_in[y * width * 3 + x - width * 3], 
                img_in[y * width * 3 + x - (width + 1) * 3]
                );
            img_out[y * width * 3 + x] = grayblurvalue;
            img_out[y * width * 3 + x + 1] = grayblurvalue;
            img_out[y * width * 3 + x + 2] = grayblurvalue;

        }
    }
}


void blurrbg(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

    //inner box
    for (int y = 1; y < height - 1; ++y){
        for(int x = 3; x < width * 3 - 3; x += 3){
            for(int rbg = 0; rbg < 3; ++rbg){
                img_out[y * width * 3 + x + rbg] = gaussianblur3x3(
                    img_in[y * width * 3 + x + (width - 1) * 3 + rbg], 
                    img_in[y * width * 3 + x + width * 3 + rbg], 
                    img_in[y * width * 3 + x + (width + 1) * 3 + rbg], 
                    img_in[y * width * 3 + x - 3 + rbg], 
                    img_in[y * width * 3 + x + rbg], 
                    img_in[y * width * 3 + x + 3 + rbg], 
                    img_in[y * width * 3 + x - (width - 1) * 3 + rbg], 
                    img_in[y * width * 3 + x - width * 3 + rbg], 
                    img_in[y * width * 3 + x - (width + 1) * 3 + rbg]
                    );
            }
        }
    }

}


uint8_t gaussianblur3x3(uint8_t topleft, uint8_t topmid, uint8_t topright, uint8_t left, uint8_t mid, uint8_t right, uint8_t bottomleft, uint8_t bottommid, uint8_t bottomright){
    return (topleft >> 4)  + (topmid >> 3) + (topright >> 4) + (left >> 3) + (mid >> 2) + (right >> 3) + (bottomleft >> 4) + (bottommid >> 3) + (bottomright >> 4);
}