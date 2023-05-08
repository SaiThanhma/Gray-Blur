#pragma once
#include <cstdio>
#include <algorithm>
#include <tuple>
#include <memory>
#include <cmath>
#include <vector>
#include <iostream>

using Kernel = std::vector<std::vector<float>>;
using Coordinates = std::vector<std::pair<int, int>>;

struct Index
{
    static constexpr auto mirrorindex = [](int height, int width, int kernelheight, int kernelwidth)
    {
        int endy = height - 1;
        int endx = width - 1;

        int kernelwidthradius = (kernelwidth - 1) / 2;
        int kernelheightradius = (kernelheight - 1) / 2;

        int nheight = height + kernelheightradius * 2;
        int nwidth = width + kernelwidthradius * 2;

        int size = (2 * kernelwidthradius + width) * (2 * kernelheightradius + height);
        Coordinates c;
        c.reserve(size);

        int i = 0;
        for (int posy = -kernelheightradius; posy < nheight - kernelheightradius; ++posy)
        {
            for (int posx = -kernelwidthradius; posx < nwidth - kernelwidthradius; ++posx, ++i)
            {
                if ((posx >= 0 && posx <= endx) && (posy >= 0 && posy <= endy))
                {
                    c.emplace_back(posy, posx);
                    continue;
                }

                int distancey = posy < 0 ? -posy : posy;
                int repy = distancey / endy;
                int mody = distancey % endy;
                int indexy = repy % 2 == 0 ? mody : endy - mody;

                int distancex = posx < 0 ? -posx : posx;
                int repx = distancex / endx;
                int modx = distancex % endx;
                int indexx = repx % 2 == 0 ? modx : endx - modx;

                c.emplace_back(indexy, indexx);
            }
        }

        return c;
    };
};

template <typename T>
constexpr void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel);

template <typename T>
constexpr void kernelCalculation(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int i, int j, const Kernel &kernel, Coordinates &coordinates, int shift);

template <typename T>
constexpr void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel)
{
    int kernelheight = kernel.size();
    int kernelwidth = kernel.at(0).size();

    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;
    int nwidth = width + kernelwidthradius * 2;
    int shift = nwidth * kernelheightradius + kernelwidthradius;

    Coordinates c = Index::mirrorindex(height, width, kernelheight, kernelwidth);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            kernelCalculation<T>(img_in, width, height, channels, img_out, i, j, kernel, c, shift);
        }
    }
}

template <typename T>
constexpr void kernelCalculation(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int i, int j, const Kernel &kernel, Coordinates &coordinates, int shift)
{

    int kernelwidth = kernel.at(0).size();
    int kernelheight = kernel.size();

    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;
    int nwidth = width + kernelwidthradius * 2;

    for (int k = 0; k < channels; ++k)
    {
        float sum = 0;
        int pos = (i * nwidth + j);
        int shiftkernel = pos - kernelwidthradius + nwidth * kernelheightradius;

        for (int l = 0; l < kernel.size(); ++l, shiftkernel -= nwidth)
        {
            for (int m = 0; m < kernel.at(0).size(); ++m)
            {
                int y = coordinates.at(shift + shiftkernel + m).first;
                int x = coordinates.at(shift + shiftkernel + m).second;

                sum += img_in[y * width * channels + x * channels + k] * kernel.at(l).at(m);
            }
        }

        img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
    }
}
