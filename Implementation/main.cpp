#include "sobel.h"
#include "gaussian.h"
#include "convolution.h"
#include "io.h"
#include "Image/image.h"
#include "bitmap.h"
#include <iostream>
#include <string>
#include <getopt.h>
#include <iomanip>
#include <chrono>


static void usage(char *argv0)
{
    std::cout
        << "Usage: " << argv0 << " input output [-gray [\"r g b\"]] [-gaussian [\"seperate kSize border\"]] [-sobel [\"kSize gausskSize threshold gradient\"]]"
        << "\n"
        << "input: input file .bmp as extension."
        << "\n"
        << "output: output file with .bmp as extension, otherwise a .bmp file will be created."
        << "\n"
        << "gray: grayscale filter with \"r g b\" as weighted parameters. 0 <= r, b, g <= 1 with r + g + b = 1. Default is r = 0.3, g = 0.59, b = 0.11 (luma grayscaling)"
        << "\n"
        << "gaussian: gaussian filter with \"v k b\" If seperate = 0 then the image will be processed once with a kSize xkSize Kernel in quadratic time, "
        << "otherwise image will be processed twice with a 1 x kSize and kSize x 1 Kernel in linear time. kSize has to be positive and odd. "
        << "border can be chosen between 0 | 1 | 2 | 3, 0 = WO, 1 = EXTEND 2 = MIRROR, 3 = WRAP. "
        << "Default is seperate = 1, k = 3, border = EXTEND"
        << "\n"
        << "sobel: sobel operator with a kSize x kSize Kernel, with kSize >= 3 and odd. The image will be preprocessed with a gausskSize x gausskSize gaussian Filter. "
        << "The threshold is a number for an edge to be recognized between 0 <= threshold <= 255 (ideally about 45). If gradient = 1 then the gradient (orientation) will be also shown in colors "
        << "Default is kSize = 5, gausskSize = 3, threshold = 45, gradient = 0"
        << std::endl;
}

int main(int argc, char **argv)
{
    
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    static struct option long_options[] = {
        {"gray", optional_argument, 0, 'g'},
        {"gaussian", optional_argument, 0, 'b'},
        {"sobel", optional_argument, 0, 's'},
        {0, 0, 0, 0}};

    IOhandler iohandler;
    int longindex;
    int opt;

    bool grayf = false;
    bool blurf = false;
    bool sobelf = false;

    while ((opt = getopt_long(argc, argv, "g::b::s::", long_options, &longindex)) != EOF)
    {
        switch (opt)
        {
        case 'g':
        { 
            if (optarg && !iohandler.grayHandler(optarg))
            {
                usage(argv[0]);
                return 1;
            }
            grayf = true;
            break;
        }
        case 'b':
        {
            if (optarg && !iohandler.gaussianHandler(optarg))
            {
                usage(argv[0]);
                return 1;
            }
            blurf = true;
            break;
        }
        case 's':
        {
            if (optarg && !iohandler.sobelHandler(optarg))
            {
                usage(argv[0]);
                return 1;
            }
            sobelf = true;
            break;
        }
        default:
        {
            usage(argv[0]);
            return 1;
        }
        }
    }

    if (optind >= argc || argc - optind != 2)
    {
        usage(argv[0]);
        return 1;
    }
    std::string_view input{argv[optind++]};
    std::string_view output{argv[optind]};

    if (!iohandler.pathHandler(input.data()) || !iohandler.pathHandler(output.data()))
    {
        usage(argv[0]);
        return 1;
    }
    
    std::unique_ptr<Bitmap> img_in;
    if(!(img_in = readBmpFile(input.data()))){
        return 1;
    }
    if(!copyBitmapHeader(input.data(), output.data())){
        return 1;
    }
    if(img_in->biHeight == 0 || img_in->biWidth == 0){
        std::cout << "Picture height or width is 0" << std::endl;
        return 1;
    }

    std::unique_ptr<uint8_t[]> odata = std::make_unique<uint8_t[]>(img_in->biHeight * img_in->biWidth * 3);
    if(grayf){
        auto startgray = std::chrono::system_clock::now(); 

        auto endgray = std::chrono::system_clock::now();

        float resgray = std::chrono::duration_cast<std::chrono::seconds>(endgray-startgray).count();

        std::cout << "Time for gray: " << std::setprecision(5) << resgray << std::endl;

    }
    if(blurf)
    {
        auto startblur = std::chrono::system_clock::now(); 

        auto endblur = std::chrono::system_clock::now();

        float resblur = std::chrono::duration_cast<std::chrono::seconds>(endblur-startblur).count();
        std::cout << "Time for blur: " << std::fixed <<std::setprecision(5) << resblur << std::endl;
    }
    if(sobelf){

        auto startsobel = std::chrono::system_clock::now(); 

        sobel<uint8_t>(img_in->data.get(), img_in->biWidth, img_in->biHeight, 3, odata.get());

        auto endsobel = std::chrono::system_clock::now();
        
        float ressobel = std::chrono::duration_cast<std::chrono::seconds>(endsobel-startsobel).count();

        std::cout << "Time for sobel: " << std::fixed << std::setprecision(2) << ressobel << std::endl;
    }

    return 0;
}
