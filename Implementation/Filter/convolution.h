#pragma once
#include <cstdio>
#include <tuple>
#include <memory>
#include <cmath>
#include <vector>
#include <iostream>

using Kernel = std::vector<std::vector<float>>;
using Coordinates = std::pair<int, int>;
using indexCall = Coordinates (*)(int posy, int posx, int endy, int endx);

enum Border
{
    EXTEND,
    MIRROR,
    WRAP,
    WO,
};

struct Index
{
    static constexpr int positive_modulo(int i, int n)
    {
        return (i % n + n) % n;
    }

    static constexpr Coordinates mirrorIndex(int posy, int posx, int endy, int endx)
    {
        if ((posx >= 0 && posx <= endx) && (posy >= 0 && posy <= endy))
        {
            return std::make_pair(posy, posx);
        }

        int distancey = std::abs(posy);
        int repy = distancey / endy;
        int mody = distancey % endy;
        int indexy = repy % 2 == 0 ? mody : endy - mody;

        int distancex = std::abs(posx);
        int repx = distancex / endx;
        int modx = distancex % endx;
        int indexx = repx % 2 == 0 ? modx : endx - modx;

        return std::make_pair(indexy, indexx);
    }

    static constexpr Coordinates extendIndex(int posy, int posx, int endy, int endx)
    {
        int indexy = 0;
        int indexx = 0;

        if (posy >= 0 && posy <= endy)
            indexy = posy;
        else if (posy > endy)
            indexy = endy;
        else
            indexy = 0;

        if (posx >= 0 && posx <= endx)
            indexx = posx;
        else if (posx > endx)
            indexx = endx;
        else
            indexx = 0;

        return std::make_pair(indexy, indexx);
    }

    static constexpr Coordinates wrapIndex(int posy, int posx, int endy, int endx)
    {
        int indexy = Index::positive_modulo((posy + endy + 1),(endy + 1));
        int indexx = Index::positive_modulo((posx + endx + 1),(endx + 1));
        return std::make_pair(indexy, indexx);
    }
};

template <typename T>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel, Border border);

template <typename T>
void borderHandling(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel, indexCall index);

template <typename T>
void convolutionWO(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel);

Kernel convolution(const Kernel &kernel1, const Kernel &kernel2);

template <typename T>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel, Border border)
{

    switch (border)
    {
    case EXTEND:
        convolutionWO(img_in, width, height, channels, img_out, kernel);
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::extendIndex);
        break;

    case MIRROR:
        convolutionWO(img_in, width, height, channels, img_out, kernel);
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::mirrorIndex);
        break;

    case WRAP:
        convolutionWO(img_in, width, height, channels, img_out, kernel);
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::wrapIndex);
        break;

    case WO:
        convolutionWO(img_in, width, height, channels, img_out, kernel);
        break;
    }
}

template <typename T>
void convolutionWO(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel)
{
    int kernelheight = kernel.size();
    int kernelwidth = kernel.at(0).size();

    int kernelheightradius = (kernelheight - 1) / 2;
    int kernelwidthradius = (kernelwidth - 1) / 2;

    int starty = -kernelheightradius;
    int endy = kernelheightradius;

    int startx = -kernelwidthradius;
    int endx = kernelwidthradius;

    for (int i = kernelheightradius; i < height - kernelheightradius; ++i)
    {
        for (int j = kernelwidthradius; j < width - kernelwidthradius; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                float sum = 0;
                for (int l = starty; l <= endy; ++l)
                {
                    for (int m = startx; m <= endx; ++m)
                    {
                        sum += img_in[((i + l) * width + (j + m)) * channels + k] * kernel.at(l + kernelheightradius).at(m + kernelwidthradius);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }
}

template <typename T>
void borderHandling(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel, indexCall index)
{
    int kernelheight = kernel.size();
    int kernelwidth = kernel.at(0).size();

    int kernelheightradius = (kernelheight - 1) / 2;
    int kernelwidthradius = (kernelwidth - 1) / 2;

    int starty = -kernelheightradius;
    int endy = kernelheightradius;

    int startx = -kernelwidthradius;
    int endx = kernelwidthradius;

    for (int i = 0; i < kernelheightradius; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                float sum = 0;
                for (int l = starty; l <= endy; ++l)
                {
                    for (int m = startx; m <= endx; ++m)
                    {
                        Coordinates c = index(i + l, j + m, height - 1, width - 1);
                        sum += img_in[(c.first * width + c.second) * channels + k] * kernel.at(l + kernelheightradius).at(m + kernelwidthradius);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }

    for (int i = (height - kernelheightradius); i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                float sum = 0;
                for (int l = starty; l <= endy; ++l)
                {
                    for (int m = startx; m <= endx; ++m)
                    {
                        Coordinates c = index(i + l, j + m, height - 1, width - 1);
                        sum += img_in[(c.first * width + c.second) * channels + k] * kernel.at(l + kernelheightradius).at(m + kernelwidthradius);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }

    for (int i = kernelheightradius; i < (height - kernelheightradius); ++i)
    {
        for (int j = 0; j < kernelwidthradius; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                float sum = 0;
                for (int l = starty; l <= endy; ++l)
                {
                    for (int m = startx; m <= endx; ++m)
                    {
                        Coordinates c = index(i + l, j + m, height - 1, width - 1);
                        sum += img_in[(c.first * width + c.second) * channels + k] * kernel.at(l + kernelheightradius).at(m + kernelwidthradius);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }

    for (int i = kernelheightradius; i < (height - kernelheightradius); ++i)
    {
        for (int j = width - kernelwidthradius; j < width; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                float sum = 0;
                for (int l = starty; l <= endy; ++l)
                {
                    for (int m = startx; m <= endx; ++m)
                    {
                        Coordinates c = index(i + l, j + m, height - 1, width - 1);
                        sum += img_in[(c.first * width + c.second) * channels + k] * kernel.at(l + kernelheightradius).at(m + kernelwidthradius);
                    }
                }
                img_out[(i * width + j) * channels + k] = static_cast<T>(sum);
            }
        }
    }
}

Kernel convolution(const Kernel &first, const Kernel &second){

    int kernel1height = first.size();
    int kernel1width = first.at(0).size();

    int kernel2height = second.size();
    int kernel2width = second.at(0).size();

    int kernelresultheight = kernel1height + kernel2height - 1;
    int kernelresultwidth = kernel1width + kernel2width - 1;

    Kernel res;
    res.reserve(kernelresultheight);
    int starty = -(kernelresultheight / 2);
    int endy = (kernelresultheight / 2);

    int startx = -(kernelresultwidth / 2);
    int endx = (kernelresultwidth / 2);

    for(int j = starty; j <= endy; ++j){
        std::vector<float> sub;
        for(int k = startx; k <= endx; ++k){
            float sum = 0;
            bool set = false;
            for(int p = 0; p < kernel1height; ++p){
                for(int q = 0; q < kernel1width; ++q){
                    if(j - p + 1 < 0 || j - p + 1 >= kernel2height || k - q + 1 < 0 || k - q + 1 >= kernel2width) {
                        continue;
                    }
                    set = true;
                    sum += first.at(p).at(q) * second.at(j - p + 1).at(k - q + 1);
                }
            }
            if(set){
                sub.push_back(sum);
                set = false;
            }

        }
        res.emplace_back(sub);
    }
    return res;
}
