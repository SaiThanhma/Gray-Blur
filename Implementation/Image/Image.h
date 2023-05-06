#include <string>
#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

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

    static void write(std::string&& path, int width, int height, int channels, const void *data)
    {   
        std::string format{path.substr(path.size() - 3)};
        if(format == "png"){
            stbi_write_png(path.data(), width, height, channels, data, width * channels);
        }
        else if(format == "jpg"){
            stbi_write_jpg(path.data(), width, height, channels, data, 100);
        }
        else if(format == "bmp"){
            stbi_write_bmp(path.data(), width, height, channels, data);
        }
        else if(format == "tga"){
            stbi_write_tga(path.data(), width, height, channels, data);
        }
        else{
            std::string opath{path.substr(0, path.size() - 4) + "png"};
            stbi_write_tga(opath.data(), width, height, channels, data);
        }
    }

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
    stbi_image_free(rawdata);
}