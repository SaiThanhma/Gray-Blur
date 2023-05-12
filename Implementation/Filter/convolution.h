#pragma once
#include <cstdio>
#include <algorithm>
#include <tuple>
#include <memory>
#include <cmath>
#include <vector>

using Kernel = std::vector<std::vector<float>>;
using Coordinates = std::pair<int, int>;
using indexCall = Coordinates (*)(int posy, int posx, int endy, int endx);

enum Border
{
    WO,
    EXTEND,
    MIRROR,
    WRAP,
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
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, Kernel kernel, Border border);

template <typename T>
void borderHandling(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel, indexCall index);

template <typename T>
void convolutionWO(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, const Kernel &kernel);

constexpr Kernel convolution(const Kernel &a, const Kernel &b);

constexpr void reverseKernel(Kernel &kernel);

template <typename T>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, Kernel kernel, Border border)
{

    reverseKernel(kernel);
    convolutionWO(img_in, width, height, channels, img_out, kernel);

    switch (border)
    {
    case EXTEND:
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::extendIndex);
        break;

    case MIRROR:
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::mirrorIndex);
        break;

    case WRAP:
        borderHandling(img_in, width, height, channels, img_out, kernel, Index::wrapIndex);
        break;
    case WO:
        return;
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

constexpr Kernel convolution(const Kernel &a, const Kernel &b){

    int aheight = a.size();
    int awidth = a.at(0).size();

    int bheight = b.size();
    int bwidth = b.at(0).size();

    int kernelresultheight = aheight + bheight - 1;
    int kernelresultwidth = awidth + bwidth - 1;

    Kernel res;
    res.reserve(kernelresultheight);
    for(int j = 0; j < kernelresultheight; ++j){
        std::vector<float> sub;
        sub.reserve(kernelresultwidth);
        for(int k = 0; k <= kernelresultwidth ; ++k){
            float sum = 0;
            bool set = false;
            for(int p = 0; p < aheight; ++p){
                for(int q = 0; q < awidth; ++q){
                    if(j - p < 0 || j - p >= bheight || k - q < 0 || k - q >= bwidth) {
                        continue;
                    }
                    set = true;
                    sum += a.at(p).at(q) * b.at(j - p).at(k - q);
                }
            }
            if(set){
                sub.push_back(sum);
                set = false;
            }

        }
        if(sub.size() != 0){
        res.push_back(sub);
        }
    }
    return res;
}


constexpr void reverseKernel(Kernel &kernel){

    for(auto &i : kernel){
        std::reverse(i.begin(), i.end());
    }
    std::reverse(kernel.begin(), kernel.end());
}