#pragma once
#include <cstdio>
#include <algorithm>
#include <tuple>
#include <memory>
#include <cmath>
#include <vector>
#include <iostream>

using Kernel = std::vector<std::vector<float>>;
using Coordinates = std::pair<int, int>;

struct Index
{
    static constexpr std::pair<int, int> mirrorindex(int posy, int posx, int endy, int endx)
    {
        if ((posx >= 0 && posx <= endx) && (posy >= 0 && posy <= endy))
        {
            return std::make_pair(posy, posx);
        }

        int distancey = posy < 0 ? -posy : posy;
        int repy = distancey / endy;
        int mody = distancey % endy;
        int indey = repy % 2 == 0 ? mody : endy - mody;

        int distancex = posx < 0 ? -posx : posx;
        int repx = distancex / endx;
        int modx = distancex % endx;
        int index = repx % 2 == 0 ? modx : endx - modx;

        return std::make_pair(indey, index);
    }
};

template <typename T>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel);

template <typename T>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel)
{
    int kernelheight = kernel.size();
    int kernelwidth = kernel.at(0).size();

    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                float sum = 0;
                int shifty = i + kernelheightradius;
                int shiftx = j - kernelwidthradius;
                for (int l = 0; l < kernelheight; ++l, --shifty, shiftx = j - kernelwidthradius)
                {
                    for (int m = 0; m < kernelwidth; ++m, ++shiftx)
                    {
                        Coordinates c = Index::mirrorindex(shifty, shiftx, height - 1, width - 1);
                        sum += img_in[c.first * width * channels + c.second * channels + k] * kernel.at(l).at(m);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }
}
