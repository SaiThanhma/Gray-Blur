#pragma once
#include "gaussian.h"
#include "gray.h"
#include <tuple>
#include <cstdio>
#include <cinttypes>
#include <memory>
#include <vector>
#include <cmath>

using Color = std::tuple<uint8_t, uint8_t, uint8_t>;

template <typename T>
void sobel(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize = 3, size_t gaussianKsize = 3, size_t threshold = 0, bool showgradient = false);

template <typename T>
void getGyGx(const T *img_in, size_t width, size_t height, size_t channels, float *img_outY, float *img_outX, size_t ksize, size_t gaussianKsize);

template <typename T>
void magnitude(const float *gy, const float *gx, size_t width, size_t height, size_t channels, T *img_out, size_t threshold);

template <typename T>
void gradient(const float *gy, const float *gx, size_t width, size_t height, size_t channels, T *img_out, size_t threshold);

template <typename T>
void copy(const T *img_in, float *img_inX, float *img_inY, size_t size);

constexpr std::pair<Kernel, Kernel> sobelKernel(size_t ksize);

constexpr void normalizeKernel(Kernel &first, Kernel &second);

template <typename T>
void rescaleLightDark(const float *img_in, size_t width, size_t height, size_t channels, T *img_out);

Color colorGradient(double angle);

template <typename T>
void sobel(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize, size_t gaussianKsize, size_t threshold, bool showgradient)
{
    size_t size = width * height * channels;

    std::unique_ptr<float[]> gy = std::make_unique<float[]>(size);
    std::unique_ptr<float[]> gx = std::make_unique<float[]>(size);

    getGyGx(img_in, width, height, channels, gy.get(), gx.get(), ksize, gaussianKsize);

    if (showgradient)
    {
        gradient(gy.get(), gx.get(), width, height, channels, img_out, threshold);
    }
    else
    {
        std::unique_ptr<float[]> tmpres = std::make_unique<float[]>(size);
        magnitude(gy.get(), gx.get(), width, height, channels, tmpres.get(), threshold);
        rescaleLightDark(tmpres.get(), width, height, channels, img_out);
    }
}

template <typename T>
void getGyGx(const T *img_in, size_t width, size_t height, size_t channels, float *img_outY, float *img_outX, size_t ksize, size_t gaussianKsize)
{
    size_t size = width * height * channels;

    std::unique_ptr<T[]> img_ingray = std::make_unique<T[]>(size);
    std::unique_ptr<T[]> img_ingrayblur = std::make_unique<T[]>(size);
    gray<T>(img_in, width, height, channels, img_ingray.get(), 0.33f, 0.33f, 0.33f);
    gaussianBlurSeparate(img_ingray.get(), width, height, channels, img_ingrayblur.get(), gaussianKsize, EXTEND);

    std::unique_ptr<float[]> img_inY = std::make_unique<float[]>(size);
    std::unique_ptr<float[]> img_inX = std::make_unique<float[]>(size);

    auto kernelpair = sobelKernel(ksize);
    Kernel sy = kernelpair.first;
    Kernel sx = kernelpair.second;

    copy<T>(img_ingrayblur.get(), img_inX.get(), img_inY.get(), size);

    convolution<float>(img_inY.get(), width, height, channels, img_outY, sy, EXTEND);
    convolution<float>(img_inX.get(), width, height, channels, img_outX, sx, EXTEND);
}

template <typename T>
void magnitude(const float *gy, const float *gx, size_t width, size_t height, size_t channels, T *img_out, size_t threshold)
{
    size_t size = width * height * channels;
    for (size_t i = 0; i < size - channels + 1; i += channels)
    {
        // img_out[i] = static_cast<uint8_t>(std::sqrt(std::pow((img_outX.get()[i]), 2) + std::sqrt(std::pow(img_outY.get()[i], 2))));
        // abs(x) + abs(y) ≈ sqrt(x^2 + y), since abs is more efficent

        img_out[i] = std::abs(gy[i]) + std::abs(gx[i]);
        if (img_out[i] > static_cast<float>(threshold))
        {
            img_out[i + 1] = img_out[i];
            img_out[i + 2] = img_out[i];
        }
        else
        {
            img_out[i] = 0;
            img_out[i + 1] = img_out[i];
            img_out[i + 2] = img_out[i];
        }
    }
}

template <typename T>
void gradient(const float *gy, const float *gx, size_t width, size_t height, size_t channels, T *img_out, size_t threshold)
{
    size_t size = width * height * channels;
    for (size_t i = 0; i < size - channels + 1; i += channels)
    {

        // img_out[i] = static_cast<uint8_t>(std::sqrt(std::pow((img_outX.get()[i]), 2) + std::sqrt(std::pow(img_outY.get()[i], 2))));
        // abs(x) + abs(y) ≈ sqrt(x^2 + y), since abs is more efficent

        img_out[i] = static_cast<T>((std::abs(gy[i]) + std::abs(gx[i])));

        if (img_out[i] > threshold)
        {
            double angle = std::atan2(gy[i], gx[i]);
            Color c = colorGradient(angle);
            img_out[i] = std::get<0>(c);
            img_out[i + 1] = std::get<1>(c);
            img_out[i + 2] = std::get<2>(c);
        }
        else
        {
            img_out[i] = 0;
            img_out[i + 1] = img_out[i];
            img_out[i + 2] = img_out[i];
        }
    }
}

template <typename T>
void copy(const T *img_in, float *img_inX, float *img_inY, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        img_inX[i] = img_in[i];
        img_inY[i] = img_in[i];
    }
}

constexpr std::pair<Kernel, Kernel> sobelKernel(size_t ksize)
{
    Kernel sy{{1.f, 2.f, 1.f}, {0.f, 0.f, 0.f}, {-1.f, -2.f, -1.f}};
    Kernel sx{{1.f, 0.f, -1.f}, {2.f, 0.f, -2.f}, {1.f, 0.f, -1.f}};
    if (ksize <= 3 || ksize % 2 == 0)
    {
        normalizeKernel(sy, sx);
        return std::make_pair(sy, sx);
    }

    Kernel smooth = convolution({{1.f / 4}, {2.f / 4}, {1.f / 4}}, {{1.f / 4, 2.f / 4, 1.f / 4}});
    Kernel syres = sy;
    Kernel sxres = sx;

    for (size_t i = 3; i < ksize; i += 2)
    {
        syres = convolution(smooth, syres);
        sxres = convolution(smooth, sxres);
    }

    return std::make_pair(syres, sxres);
}

constexpr void normalizeKernel(Kernel &first, Kernel &second)
{
    // normalize 3x3 Kernel by 1/6 to prevent overflows
    float inv = 1.f / 6.f;
    for (size_t i = 0; i < first.size(); ++i)
    {
        for (size_t j = 0; j < first.at(0).size(); ++j)
        {
            first.at(i).at(j) *= inv;
            second.at(i).at(j) *= inv;
        }
    }
}

Color colorGradient(double angle)
{
    double Pi2 = 2 * M_PI;

    if (angle < 0.0)
        for (; angle < 0.0; angle += Pi2)
            ;
    else if (angle > Pi2)
        for (; angle > Pi2; angle -= Pi2)
            ;

    int currentcolor = static_cast<int>((angle / Pi2) * 6);
    int norm = static_cast<int>((angle / Pi2) * 255 * 6);
    int shift = norm % 255;

    switch (currentcolor)
    {
    case 0: // red
        return {255, shift, 0};
    case 1: // yellow
        return {255 - shift, 255, 0};
    case 2: // green
        return {0, 255, shift};
    case 3: // cyan
        return {0, 255 - shift, 255};
    case 4: // blue
        return {shift, 0, 255};
    case 5: // magenta
        return {255, 0, 255 - shift};
    default:
        return {0, 0, 0};
    }
}

template <typename T>
void rescaleLightDark(const float *img_in, size_t width, size_t height, size_t channels, T *img_out)
{
    int max = -100;
    size_t size = width * height * channels;

    for (size_t i = 0; i < size - channels + 1; i += channels)
    {
        int tmp = static_cast<int>(img_in[i]);
        if (tmp > max)
        {
            max = tmp;
        }
    }

    float ratio = 255.f / static_cast<float>(max);

    for (size_t i = 0; i < size - channels + 1; i += channels)
    {
        img_out[i] = static_cast<T>(img_in[i] * ratio);
        img_out[i + 1] = img_out[i];
        img_out[i + 2] = img_out[i];
    }
}