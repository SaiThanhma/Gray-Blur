#pragma once
#include "gaussian.h"
#include "gray.h"
#include <tuple>
#include <cstdio>
#include <cinttypes>
#include <memory>
#include <array>
#include <cmath>

using Color = std::tuple<int, int, int>;

static const std::array<float, 9> sx{{1.f, 0.f, -1.f, 2.f, 0.f, -2.f, 1.f, 0.f, -1.f}};
static const std::array<float, 9> sy{{1.f, 2.f, 1.f, 0.f, 0.f, 0.f, -1.f, -2.f, -1.f}};

template <typename T>
void sobel(const T *img_in, size_t width, size_t height, size_t channels, T *img_out);

template <typename T>
void squish(const T *img_in, float *img_inX, float *img_inY, size_t size);

Color colorGradient(double angle);

template <typename T>
void sobel(const T *img_in, size_t width, size_t height, size_t channels, T *img_out)
{
    size_t size = width * height * channels;
    std::unique_ptr<T[]> img_ingray = std::make_unique<T[]>(size);
    std::unique_ptr<T[]> img_ingrayblur = std::make_unique<T[]>(size);

    gray<T>(img_in, width, height, channels, img_ingray.get());
    gaussianBlur<T, 3>(img_ingray.get(), width, height, channels, img_ingrayblur.get());

    std::unique_ptr<float[]> img_inX = std::make_unique<float[]>(size);
    std::unique_ptr<float[]> img_outX = std::make_unique<float[]>(size);
    std::unique_ptr<float[]> img_inY = std::make_unique<float[]>(size);
    std::unique_ptr<float[]> img_outY = std::make_unique<float[]>(size);
    squish<T>(img_ingrayblur.get(), img_inX.get(), img_inY.get(), size);

    convolution<float, 3, 3, true>(img_inX.get(), width, height, channels, img_outX.get(), sx);
    convolution<float, 3, 3, true>(img_inY.get(), width, height, channels, img_outY.get(), sy);

    for (int i = 0; i < size - 2; i += 3)
    {

        //   img_out[i] = static_cast<uint8_t>(std::sqrt(std::pow((img_outX.get()[i]), 2) + std::sqrt(std::pow(img_outY.get()[i], 2))));
        img_out[i] = static_cast<uint8_t>((std::abs(img_outX.get()[i]) + std::abs(img_outY.get()[i])));
        double angle = std::atan2(img_outY.get()[i], img_outX.get()[i]);
        if (img_out[i] > 35)
        {
            Color c = colorGradient(angle);
            img_out[i] = std::get<0>(c);
            img_out[i + 1] = std::get<1>(c);
            img_out[i + 2] = std::get<2>(c);
        }
        else
        {
            img_out[i + 1] = img_out[i];
            img_out[i + 2] = img_out[i];
        }
    }
}

template <typename T>
void squish(const T *img_in, float *img_inX, float *img_inY, size_t size)
{
    float value;
    for (int i = 0; i < size; ++i)
    {
        value = static_cast<float>(img_in[i]) * (1.f / 6.f);
        img_inX[i] = value;
        img_inY[i] = value;
    }
}

Color colorGradient(double angle)
{
    double Pi2 = 2 * M_PI;

    if (angle < 0.0) for (; angle < 0.0; angle += Pi2);
    else if (angle > Pi2) for (; angle > Pi2; angle -= Pi2);

    int currentcolor = (angle / Pi2) * 6;
    int norm = (angle / Pi2) * 255 * 6;
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
