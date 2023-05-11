#pragma once
#include "convolution.h"
#include <cinttypes>
#include <cstdio>
#include <vector>
#include <memory>
#include <cmath>

using Kernel = std::vector<std::vector<float>>;

template <typename T>
void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int ksize, Border border);

template <typename T>
void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int ksize, Border border);

constexpr Kernel gaussianKernel(int kernelheight, int kernelwidth, float sigma);

constexpr float sigmaToksize(int ksize);

constexpr float hg(int i, int j, int meani, int meanj, float sigma);

template <typename T>
void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int ksize, Border border)
{
   float sigma = sigmaToksize(ksize);
   auto kernel = gaussianKernel(ksize, ksize, sigma);
   convolution<T>(img_in, width, height, channels, img_out, kernel, border);
}

template <typename T>
void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, int ksize, Border border)
{
    float sigma = sigmaToksize(ksize);
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(width * height * channels);
    auto kernelhorizontal = gaussianKernel(1, ksize, sigma);
    auto kernelvertical = gaussianKernel(ksize,1 ,sigma);
    convolution<T>(img_in, width, height, channels, tmp.get(), kernelhorizontal, border);
    convolution<T>(tmp.get(), width, height, channels, img_out, kernelvertical, border);
}

constexpr Kernel gaussianKernel(int kernelheight, int kernelwidth, float sigma)
{
    Kernel kernel;
    kernel.reserve(kernelheight);
    float sum = 0.0;
    float tmp = 0.0;
    int meani = kernelheight / 2;
    int meanj = kernelwidth / 2;

    for (int i = 0; i < kernelheight; ++i)
    {
        std::vector<float> sub;
        sub.reserve(kernelwidth);
        for (int j = 0; j < kernelwidth; ++j)
        {
            tmp = hg(i, j, meani, meanj, sigma);
            sum += tmp;
            sub.emplace_back(tmp);
        }
        kernel.emplace_back(std::move(sub));
    }

    if (sum != 0.0)
    {
        for (int i = 0; i < kernelheight; ++i)
        {
            for (int j = 0; j < kernelwidth; ++j)
            {
                kernel.at(i).at(j) /= sum;
            }
        }
    }

    return kernel;
}

constexpr float sigmaToksize(int ksize)
{
    return 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8;
}

constexpr float hg(int i, int j, int meani, int meanj, float sigma)
{
    return std::exp(-0.5 * ((i - meani) * (i - meani) + (j - meanj) * (j - meanj)) / (sigma * sigma));
}
