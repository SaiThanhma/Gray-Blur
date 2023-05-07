#pragma once
#include "convolution.h"
#include <cinttypes>
#include <cstdio>
#include <array>
#include <memory>
#include <cmath>

template <typename T, int ksize>
constexpr void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out);

template <typename T, int ksize>
constexpr void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out);

template <int kernelheight, int kernelwidth>
constexpr std::array<float, kernelheight * kernelwidth> gaussianKernel(float sigma);

constexpr float sigmaToksize(int ksize);

constexpr float hg(int i, int j, int meani, int meanj, float sigma);


template <typename T, int ksize>
constexpr void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out)
{
    float sigma = sigmaToksize(ksize);
    auto kernel = gaussianKernel<ksize, ksize>(sigma);
    convolution<T, ksize, ksize>(img_in, width, height, channels, img_out, kernel);
}

template <typename T, int ksize>
constexpr void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out)
{
    float sigma = sigmaToksize(ksize);
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(width * height * channels);
    auto kernelvertical = gaussianKernel<1, ksize>(sigma);
    auto kernelhorizontal = gaussianKernel<ksize, 1>(sigma);
    convolution<T, 1, ksize>(img_in, width, height, channels, tmp.get(), kernelvertical);
    convolution<T, ksize, 1>(tmp.get(), width, height, channels, img_out, kernelhorizontal);
}

template <int kernelheight, int kernelwidth>
constexpr std::array<float, kernelheight * kernelwidth> gaussianKernel(float sigma)
{
    std::array<float, kernelheight * kernelwidth> kernel;
    float sum = 0.0;
    float tmp = 0.0;
    int meani = kernelheight / 2;
    int meanj = kernelwidth / 2;
    int pos = 0;

    for (int i = 0; i < kernelheight; ++i)
    {
        for (int j = 0; j < kernelwidth; ++j, ++pos)
        {
            tmp = hg(i, j, meani, meanj, sigma);
            sum += tmp;
            kernel.at(pos) = tmp;
        }
    }
    if (sum != 0.0)
    {
        for (int i = 0; i < kernel.size(); ++i)
        {
            kernel.at(i) /= sum;
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
