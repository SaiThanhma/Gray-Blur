#pragma once
#include <cstdio>
#include <cinttypes>
#include <memory>

template<typename T>
void gray(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, float a = 0.3f, float b = 0.59f, float c = 0.11f);

template<typename T>
void gray(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, float a, float b, float c)
{
    size_t size = width * height * channels;
    T avg;
    for(int i = 0; i < size; i+=channels){
        avg = static_cast<T>(img_in[i] * a + img_in[i + 1] * b + img_in[i + 2] * c);
        img_out[i + 0] = avg;
        img_out[i + 1] = avg;
        img_out[i + 2] = avg;
    }
    
}