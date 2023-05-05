#include <array>
#include <cmath>
#include <cinttypes>
#include <cstdio>
#include "Convolution.h"


template <typename T, int ksize>
constexpr void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out);

template <int kernelheight, int kernelwidth, double sigma>
constexpr std::array<float, kernelheight * kernelwidth> gaussianKernel();

constexpr float hg(int i, int j, int meani, int meanj, int sigma);


template <typename T, int ksize>
constexpr void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out){
    //(0.3*((ksize-1)*0.5 - 1) + 0.8)
    auto kernel = gaussianKernel<5, 5, 5.0>();
//    convolution<T, ksize, ksize>(img_in, width, height, channels, img_out, kernel);
}

template <int kernelheight, int kernelwidth, double sigma>
constexpr std::array<float, kernelheight * kernelwidth> gaussianKernel()
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
    if(sum != 0.0){
        for (int i = 0; i < kernel.size(); ++i)
        {
            kernel.at(i) /= sum;
        }
    }

    return kernel;
}

constexpr float hg(int i, int j, int meani, int meanj, double sigma)
{
    return std::exp(-0.5 * ((i - meani) * (i - meani) + (j - meanj) * (j - meanj)) / (sigma * sigma));
}
