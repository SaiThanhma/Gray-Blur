#include "image.h"

void writeImg(std::string path, int width, int height, int channels, const void *data)
{
    std::string format{path.substr(path.size() - 3)};
    if (format == "png")
    {
        stbi_write_png(path.data(), width, height, channels, data, width * channels);
    }
    else if (format == "jpg")
    {
        stbi_write_jpg(path.data(), width, height, channels, data, 100);
    }
    else if (format == "bmp")
    {
        stbi_write_bmp(path.data(), width, height, channels, data);
    }
    else if (format == "tga")
    {
        stbi_write_tga(path.data(), width, height, channels, data);
    }
    else
    {
        std::string opath{path.substr(0, path.size() - 4) + "png"};
        stbi_write_tga(opath.data(), width, height, channels, data);
    }
}