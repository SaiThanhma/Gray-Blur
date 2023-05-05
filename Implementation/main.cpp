#include "BitmapFileIO.h"
#include "Grayblur.h"
#include "Gaussian.h"
#include "Image/Image.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


int main(int argc, char const *argv[])
{

/*
    std::unique_ptr<Bitmap> bmp = readFile("../Pictures/scrambled_egg.bmp");

    copyBitmapFile("../Pictures/scrambled_egg.bmp", "../Pictures/scrambled_egg_blur.bmp");
    
    std::unique_ptr<Bitmap> convbmp = readFile("../Pictures/scrambled_egg.bmp");
    std::unique_ptr<uint8_t[]> tmp = std::make_unique<uint8_t[]>(bmp->biWidth * bmp->biHeight * 3);

    convolution<1,25>(bmp->pixeldata.get(), bmp->biWidth, bmp->biHeight, tmp.get(), gaussian_filter<1,25, 10>());
    convolution<25,1>(tmp.get(), bmp->biWidth, bmp->biHeight, convbmp->pixeldata.get(), gaussian_filter<25,1, 10>());
    writeFile("../Pictures/scrambled_egg_blur.bmp", convbmp->pixeldata.get(), convbmp->biHeight * convbmp->biWidth * 3, bmp->bfOffBits, SEEK_SET);

    */
/*
   Image<uint8_t> image{"../Pictures/scrambled_egg.bmp"};
   uint8_t *out = new uint8_t[image.width * image.height * 3];
   convolution<uint8_t, 9, 9>(image.data.get(), image.width, image.height, 3, out, gaussian_filter<9,9,5>());

   stbi_write_bmp("../Pictures/scrambled_eggtest.bmp",  image.width, image.height, 3, out);

*/
    Image<uint8_t> image{"../Pictures/baboon.png"};
    uint8_t *out = new uint8_t[image.width * image.height * image.channels];
    
    gaussianBlur<uint8_t,21>(image.data.get(), image.width, image.height, image.channels, out);



    stbi_write_png("../Pictures/baboon_blur.png",  image.width, image.height, image.channels, out, image.channels * image.width);
    return 0;
}
