#include "sobel.h"
#include "gaussian.h"
#include "convolution.h"
#include "io.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include <iostream>
#include <string>
#include <getopt.h>
#include <chrono>

static void usage(char *argv0)

    std::cout
        << "Usage: " << argv0 << " input output [-gray\"r g b\"] | [-gaussian\"seperate kSize border\"] | [-sobel\"kSize gausskSize threshold gradient\"]"
        << "\n"
        << "input: input file with [.jpg, | .png | .bmp] as possible extension."
        << "\n"
        << "output: output file with [.jpg, | .png | .bmp] as possible extension. The image will be saved as .png if the extension is unknown."
        << "\n"
        << "gray: grayscale filter with \"r g b\" as weighted parameters. 0 <= r, b, g <= 1 with r + g + b = 1. Default is r = 0.3, g = 0.59, b = 0.11 (luma grayscaling)"
        << "\n"
        << "gaussian: gaussian filter with \"gaussian kSize border\" If seperate = 0 then the image will be processed once with a kSize x kSize Kernel in quadratic time, "
        << "otherwise image will be processed twice with a 1 x kSize and a kSize x 1 kernel in linear time. kSize has to be positive and odd. "
        << "border can be chosen between 0 | 1 | 2 | 3, 0 = WO, 1 = EXTEND 2 = MIRROR, 3 = WRAP. "
        << "Default is seperate = 1, k = 3, border = EXTEND"
        << "\n"
        << "sobel: sobel operator with a kSize x kSize Kernel, with kSize >= 3 and odd. The image will be preprocessed with a grayfilter and a gausskSize x gausskSize gaussian Filter. "
        << "The threshold is a scalable number that reduces the amount of noise in the image. The range of the threshold can be set between 0 <= threshold <= 255. If gradient = 1 then the gradient (orientation) will be also shown in colors. "
        << "Default is kSize = 5, gausskSize = 3, threshold = 150, gradient = 0"
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
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    IOhandler iohandler;
    int longindex;
    int opt;

    bool grayf = false;
    bool blurf = false;
    bool sobelf = false;

    while ((opt = getopt_long(argc, argv, "hg::b::s::", long_options, &longindex)) != EOF)
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
            if (grayf || blurf || sobelf)
            {
                std::cout << "Use only one flag to to process an image" << std::endl;
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
            if (grayf || blurf || sobelf)
            {
                std::cout << "Use only one flag to to process an image" << std::endl;
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
            if (grayf || blurf || sobelf)
            {
                std::cout << "Use only one flag to to process an image" << std::endl;
                return 1;
            }
            sobelf = true;
            break;
        }
        case 'h':
        {
            usage(argv[0]);
            return 0;
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

    Image<uint8_t> img{input};
    if (img.data)
    {
    }

    if (img.width == 0 || img.height == 0)
    {
        std::cout << "Picture height or width is 0" << std::endl;
        return 1;
    }

    std::unique_ptr<uint8_t[]> odata = std::make_unique<uint8_t[]>(img.height * img.width * img.channels);

    if (grayf)
    {
        auto startgray = std::chrono::steady_clock::now();

        gray<uint8_t>(img.data.get(), img.width, img.height, img.channels, odata.get(), iohandler.r, iohandler.g, iohandler.b);

        auto endgray = std::chrono::steady_clock::now();

        std::cout << "Time for gray: " << std::chrono::duration_cast<std::chrono::milliseconds>(endgray - startgray).count() << "[µs]" << std::endl;
    }
    if (blurf)
    {
        if (iohandler.seperate)
        {

            auto startblur = std::chrono::steady_clock::now();

            gaussianBlurSeparate<uint8_t>(img.data.get(), img.width, img.height, img.channels, odata.get(), iohandler.ksize, Border(iohandler.border));

            auto endblur = std::chrono::steady_clock::now();

            std::cout << "Time for seperate blur: " << std::chrono::duration_cast<std::chrono::milliseconds>(endblur - startblur).count() << "[µs]" << std::endl;
        }
        else
        {

            auto startblur = std::chrono::steady_clock::now();

            gaussianBlur<uint8_t>(img.data.get(), img.width, img.height, img.channels, odata.get(), iohandler.ksize, Border(iohandler.border));

            auto endblur = std::chrono::steady_clock::now();

            std::cout << "Time for blur: " << std::chrono::duration_cast<std::chrono::milliseconds>(endblur - startblur).count() << "[µs]" << std::endl;
        }
    }
    if (sobelf)
    {

        auto startsobel = std::chrono::steady_clock::now();

        sobel<uint8_t>(img.data.get(), img.width, img.height, img.channels, odata.get(), iohandler.sobelkSize, iohandler.gausskSize, iohandler.threshold, iohandler.gradient);

        auto endsobel = std::chrono::steady_clock::now();

        std::cout << "Time for sobel: " << std::chrono::duration_cast<std::chrono::milliseconds>(endsobel - startsobel).count() << "[µs]" << std::endl;
    }

    writeImg(output.data(), img.width, img.height, img.channels, odata.get());
    return 0;
}
