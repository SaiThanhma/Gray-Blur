#include <cinttypes>
#include <memory>
#include <stdexcept>

class Bitmap
{
public:
    uint16_t bfType;
    uint32_t bfOffBits;
    int32_t biWidth;
    int32_t biHeight;
    std::unique_ptr<uint8_t[]> pixeldata;
};

