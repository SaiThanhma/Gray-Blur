#include <memory>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include "Bitmap.h"

std::unique_ptr<Bitmap> readFile(const char* path);
bool writeFile(const char* path, uint8_t* buffer, size_t size, long int offset, int whence);
bool copyBitmapFile(const char* in, const char* out);
