#include "sobel.h"
#include "gaussian.h"
#include "convolution.h"
#include "Image/image.h"
#include <iostream>
#include <string>
#include <getopt.h>

static void usage(char *argv0){
    std::cout 
    << "Usage: " << argv0 << " input output [-gray] [-gaussian k] [-sobel -g]" 
    << "\n"
    << "input: input file with [.jpg | .png | .bmp | .tga] as extension"
    << "\n"
    << "output: output file with [.jpg | .png | .bmp | .tga] as extension, otherwise output file will be png"
    << "\n"
    << "gray: grayscale filter with luminance as default"
    << "\n"
    << "gaussian: gaussian filter with kernel size k as parameter"
    << "\n"
    << "sobel: sobel operator with a 3x3 kernel and g flag as optional flag for additional color gradient"    
    << std::endl;
}

int main(int argc, char**argv)
{
/*
    std::unique_ptr<Bitmap> bmp = readFile("../Pictures/scrambled_egg.bmp");

    copyBitmapFile("../Pictures/scrambled_egg.bmp", "../Pictures/scrambled_egg_blur.bmp");
    
    std::unique_ptr<Bitmap> convbmp = readFile("../Pictures/scrambled_egg.bmp");
    std::unique_ptr<uint8_t[]> tmp = std::make_unique<uint8_t[]>(bmp->biWidth * bmp->biHeight * 3);

    convolution<1,25>(bmp->pixeldata.get(), bmp->biWidth, bmp->biHeight, tmp.get(), gaussian_filter<1,25, 10>());
    convolution<25,1>(tmp.get(), bmp->biWidth, bmp->biHeight, convbmp->pixeldata.get(), gaussian_filter<25,1, 10>());
    writeFile("../Pictures/scrambled_egg_blur.bmp", convbmp->pixeldata.get(), convbmp->biHeight * convbmp->biWidth * 3, bmp->bfOffBits, SEEK_SET);

    

   Image<uint8_t> image{"../Pictures/scrambled_egg.bmp"};
   uint8_t *out = new uint8_t[image.width * image.height * 3];
   convolution<uint8_t, 9, 9>(image.data.get(), image.width, image.height, 3, out, gaussian_filter<9,9,5>());

   stbi_write_bmp("../Pictures/scrambled_eggtest.bmp",  image.width, image.height, 3, out);
*/
/*
    Image<uint8_t> image{"../Pictures/lena.png"};
    uint8_t *out = new uint8_t[image.width * image.height * image.channels];

    //gray(image.data.get(), image.width, image.height, image.channels, out);
    
    sobel<uint8_t>(image.data.get(), image.width, image.height, image.channels, out);
    //stbi_write_jpg("../Pictures/Fruits_sobel.jpeg",  image.width, image.height, image.channels, out, 100);
    
    

    stbi_write_png("../Pictures/lena_sobelgradient.png",  image.width, image.height, image.channels, out, image.channels * image.width);
        while( (c = getopt_long(argc, argv, ,&longindex)) != EOF )
    {

    }
    */
   if(argc < 3){
       usage(argv[0]);
       return 1;
   }

    static struct option long_options[] = {
        {"gray",            no_argument,          0,  'g' },
        {"gaussianblur",    optional_argument,    0,  'b' },
        {"sobel",           optional_argument,    0,  's' },
        {0,                 0,                    0,   0  }
    };
    
    int longindex;
    int opt;
    while((opt = getopt_long(argc, argv, "gbs", long_options, &longindex)) != EOF){
         switch (opt)
         {
         case 'g':
             break;
         case 'b':
            break;
         case 's':
            break;
         default:
             usage(argv[0]);
             return 1;
         }
    }

    Image<uint8_t> img{argv[1]};
    std::unique_ptr<uint8_t[]> odata = std::make_unique<uint8_t[]>(img.height * img.width * img.channels);
    sobel<uint8_t>(img.data.get(), img.width, img.height, img.channels, odata.get());
    Image<uint8_t>::write(argv[2], img.width, img.height, img.channels, odata.get()); 

    return 0;
}
