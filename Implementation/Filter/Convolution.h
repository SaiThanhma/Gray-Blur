#include <cstdio>
#include <algorithm>
#include <tuple>
#include <memory>

template <typename T, unsigned kernelheight, unsigned kernelwidth>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel);

template <typename T, unsigned kernelheight, unsigned kernelwidth>
void innerconvolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel);

template <typename T, unsigned kernelheight, unsigned kernelwidth>
void borderhandling(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel);

template <typename T, unsigned kernelheight, unsigned kernelwidth>
constexpr float kernelcalc(const T *buffer, std::array<float, kernelheight * kernelwidth> kernel);

constexpr std::pair<int, int> mirrorindex(int posx, int posy, int endx, int endy);



template <typename T, unsigned kernelheight, unsigned kernelwidth>
void convolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel){
    std::reverse(kernel.begin(), kernel.end());
    innerconvolution<T, kernelheight, kernelwidth>(img_in, width, height, channels, img_out, kernel);
    borderhandling<T, kernelheight, kernelwidth>(img_in, width, height, channels, img_out, kernel);
}


template <typename T, unsigned kernelheight, unsigned kernelwidth>
constexpr float kernelcalc(const T *buffer, std::array<float, kernelheight * kernelwidth> kernel)
{
    float sum = 0;
    for(int i = 0; i < kernel.size(); ++i){
        sum += static_cast<float>(buffer[i]) * kernel.at(i);
    }

    return sum;
}

template <typename T, unsigned kernelheight, unsigned kernelwidth>
void innerconvolution(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel)
{
    std::unique_ptr<T[]> buffer = std::make_unique<T[]>(kernelheight * kernelwidth);
    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;


    for (int i = kernelheightradius; i < height - kernelheightradius; ++i)
    {
        for (int j = kernelwidthradius; j < width - kernelwidthradius; ++j)
        {
            for (int k = 0; k < channels; ++k)
            {
                int pos = (i * width + j) * channels + k;
                int shift = (pos - kernelwidthradius * channels) + width * channels * kernelheightradius;
                int index = 0;

                for (int l = 0; l < kernelheight; ++l, shift -= channels * width)
                {
                    for (int m = 0; m < kernelwidth; ++m, ++index)
                    {
                        buffer[index] = img_in[shift + m * channels];
                    }
                }

                img_out[pos] = static_cast<T>(kernelcalc<T, kernelheight, kernelwidth>(buffer.get(), kernel));
            }
        }
    }
}

template <typename T, unsigned kernelheight, unsigned kernelwidth>
void borderhandling(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, std::array<float, kernelheight * kernelwidth> kernel)
{
    std::unique_ptr<T[]> buffer = std::make_unique<T[]>(kernelheight * kernelwidth);
    std::array<std::pair<int, int>, kernelheight * kernelwidth> indicies;

    int starty = -static_cast<int>((kernelheight + 1) / 2 - 1);
    int endy = (kernelheight + 1) / 2 - 1;

    int startx = -static_cast<int>((kernelwidth + 1) / 2 - 1);
    int endx = (kernelwidth + 1) / 2 - 1;

    int pos = 0;
    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;

    for (int i = 0; i < kernelheightradius; ++i){
        for (int j = 0; j < width; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirrorindex(i + k, j + l, height - 1, width - 1);                    
                }
            }
            for (int m = 0; m < channels; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buffer[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * channels + m];
                }

                img_out[(i * width + j) * channels + m] = static_cast<T>(kernelcalc<T, kernelheight, kernelwidth>(buffer.get(), kernel));
            }
        }
    }
    
    for(int i = (height - kernelheightradius); i < height; ++i){
        for(int j = 0; j < width; ++j,  pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirrorindex(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < channels; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buffer[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * channels + m];
                }
                img_out[(i * width + j) * channels + m] = static_cast<T>(kernelcalc<T, kernelheight, kernelwidth>(buffer.get(), kernel));
            }
        }
    }

    for(int i = kernelheightradius; i < (height - kernelheightradius); ++i){
        for(int j = 0; j < kernelwidthradius; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirrorindex(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < channels; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buffer[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * channels + m];
                }
                img_out[(i * width + j) * channels + m] = static_cast<T>(kernelcalc<T, kernelheight, kernelwidth>(buffer.get(), kernel));
            }
        }
    }

    for(int i = kernelheightradius; i < (height - kernelheightradius); ++i){
        for(int j = width - kernelwidthradius; j < width; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirrorindex(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < channels; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buffer[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * channels + m];
                }
                img_out[(i * width + j) * channels + m] = static_cast<T>(kernelcalc<T, kernelheight, kernelwidth>(buffer.get(), kernel));
            }
        }
    }       
}

constexpr std::pair<int, int> mirrorindex(int posy, int posx, int endy, int endx)
{
    if ((posx >= 0 && posx <= endx) && (posy >= 0 && posy <= endy))
    {
        return std::make_pair(posy, posx);
    }

    int distancey = posy < 0 ? -posy : posy;
    int repy = distancey / endy;
    int mody = distancey % endy;
    int indey = repy % 2 == 0 ? mody : endy - mody;

    int distancex = posx < 0 ? -posx : posx;
    int repx = distancex / endx;
    int modx = distancex % endx;
    int index = repx % 2 == 0 ? modx : endx - modx;

    return std::make_pair(indey, index);
}