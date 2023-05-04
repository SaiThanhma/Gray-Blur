#include <string>
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "iostream"

template<typename T>
struct Image
{
    std::string_view path;
    int width;
    int height;
    int channels;
    int size;
    std::unique_ptr<T[]> data;

    Image(std::string_view path);
    ~Image();

    private:
    unsigned char *rawdata;
    
};

template<typename T>
Image<T>::Image(std::string_view path): path{path}, rawdata{stbi_load(path.data(), &width, &height, &channels, 0)}
{
    if(!rawdata) throw std::runtime_error("File not found"); 
    size = width * height * channels;
    data = std::make_unique<T[]>(size);

    for(int i = 0; i < size; ++i){
        data[i] = static_cast<T>(rawdata[i]);
    }
    
}

template<typename T>
Image<T>::Image::~Image(){
    stbi_image_free(this->rawdata);
}