#pragma once
#include <memory>
#include <cinttypes>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>

struct Bitmap
{
    uint16_t bfType;
    uint32_t bfOffBits;
    int32_t biWidth;
    int32_t biHeight;
    int32_t size;
    std::unique_ptr<uint8_t[]> data;
};

std::unique_ptr<Bitmap> readBmpFile(const char* path);
bool writeBmpFile(const char* path, uint8_t* buffer, size_t size, long int offset, int whence);
bool copyBitmapHeader(const char* in, const char* out);
