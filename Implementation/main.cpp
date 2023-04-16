#include "BitmapFileIO.h"
#include "Grayblur.h"


int main(int argc, char const *argv[])
{
    copyBitmapFile("../lena.bmp", "../lena3.bmp");
    std::unique_ptr<Bitmap> bmp = readFile("../lena.bmp");
    std::unique_ptr<Bitmap> bmpcopy = readFile("../lena3.bmp");
    gray(bmp->pixeldata.get(), bmp->biWidth * bmp->biHeight, bmpcopy->pixeldata.get());
    writeFile("../lena3.bmp", bmpcopy->pixeldata.get(), bmpcopy->biHeight * bmpcopy->biWidth * 3, 54, SEEK_SET);
    
    return 0;
}
