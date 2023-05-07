#pragma once
#include <cstdio>
#include <algorithm>
#include <tuple>
#include <memory>
#include <cmath>

template <size_t _Size>
using Kernel = std::array<float, _Size>;

template <size_t _Size>
using Coordinates = std::array<std::pair<int, int>, _Size>;

template <typename T, unsigned kernelheight, unsigned kernelwidth, bool gray>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel);

template <typename T, unsigned kernelheight, unsigned kernelwidth, bool gray>
constexpr void kernelCalculation(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int i, int j, Kernel<kernelheight * kernelwidth> kernel, Coordinates<kernelheight * kernelwidth> &indicies, T *buffer);

template <typename T, unsigned kernelheight, unsigned kernelwidth>
constexpr float weightedSum(const T *buffer, std::array<float, kernelheight * kernelwidth> kernel);

constexpr std::pair<int, int> mirrorindex(int posx, int posy, int endx, int endy);

template <typename T, unsigned kernelheight, unsigned kernelwidth, bool gray>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel)
{
    std::reverse(kernel.begin(), kernel.end());
    std::unique_ptr<T[]> buffer = std::make_unique<T[]>(kernelheight * kernelwidth);
    Coordinates<kernelheight * kernelwidth> indicies;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            kernelCalculation<T, kernelheight, kernelwidth, gray>(img_in, width, height, channels, img_out, i, j, kernel, indicies, buffer.get());
        }
    }
}

template <typename T, unsigned kernelheight, unsigned kernelwidth>
constexpr float weightedSum(const T *buffer, Kernel<kernelheight * kernelwidth> kernel)
{
    float sum = 0;
    for (int i = 0; i < kernel.size(); ++i)
    {
        sum += static_cast<float>(buffer[i]) * kernel.at(i);
    }

    return sum;
}

template <typename T, unsigned kernelheight, unsigned kernelwidth, bool gray>
constexpr void kernelCalculation(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int i, int j, Kernel<kernelheight * kernelwidth> kernel, Coordinates<kernelheight * kernelwidth> &indicies, T *buffer)
{
    int bufferindex = 0;
    int starty = -static_cast<int>((kernelheight + 1) / 2 - 1);
    int endy = (kernelheight + 1) / 2 - 1;

    int startx = -static_cast<int>((kernelwidth + 1) / 2 - 1);
    int endx = (kernelwidth + 1) / 2 - 1;

    int pos = (i * width + j) * channels;

    if constexpr (gray)
    {

        for (int k = starty; k <= endy; ++k)
        {
            for (int l = startx; l <= endx; ++l, ++bufferindex)
            {
                auto index = mirrorindex(i + k, j + l, height - 1, width - 1);
                buffer[bufferindex] = img_in[(index.first * width + index.second) * channels];
            }
        }

        img_out[pos] = static_cast<T>(std::ceil(weightedSum<T, kernelheight, kernelwidth>(buffer, kernel)));
        img_out[pos + 1] = img_out[pos];
        img_out[pos + 2] = img_out[pos];
    }
    else
    {
        for (int k = starty; k <= endy; ++k)
        {
            for (int l = startx; l <= endx; ++l, ++bufferindex)
            {
                indicies.at(bufferindex) = mirrorindex(i + k, j + l, height - 1, width - 1);
            }
        }

        for (int m = 0; m < channels; ++m)
        {
            for (int n = 0; n < indicies.size(); ++n)
            {
                buffer[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * channels + m];
            }

            img_out[pos + m] = static_cast<T>(std::ceil(weightedSum<T, kernelheight, kernelwidth>(buffer, kernel)));
        }
    }
}

constexpr std::pair<int, int> mirrorindex(int posy, int posx, int endy, int endx)
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