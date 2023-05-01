#include <cstdio>
#include <cinttypes>
#include <array>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <memory>
#include <iostream>

template <unsigned kernelheight, unsigned kernelwidth>
void convolution(const uint8_t *img_in, size_t width, size_t height, uint8_t *img_out, std::array<double, kernelheight * kernelwidth> kernel);

template <unsigned kernelheight, unsigned kernelwidth>
void borderhandling(const uint8_t *img_in, size_t width, size_t height, uint8_t *img_out, std::array<double, kernelheight * kernelwidth> kernel);

template <unsigned kernelheight, unsigned kernelwidth, unsigned sigma>
constexpr std::array<double, kernelheight * kernelwidth> gaussian_filter();

constexpr double hg(int i, int j, int meani, int meanj, int sigma);

constexpr std::pair<int, int> mirror(int posx, int posy, int endx, int endy);

template <unsigned kernelheight, unsigned kernelwidth>
constexpr double kernelcalc(const uint8_t *buf, std::array<double, kernelheight * kernelwidth> kernel);

template <unsigned kernelheight, unsigned kernelwidth>
constexpr double kernelcalc(const uint8_t *buf, std::array<double, kernelheight * kernelwidth> kernel)
{
    double sum = 0;
    for(int i = 0; i < kernel.size(); ++i){
        sum += buf[i] * kernel.at(i);
    }

    return sum;
}

template <unsigned kernelheight, unsigned kernelwidth>
void convolution(const uint8_t *img_in, size_t width, size_t height, uint8_t *img_out, std::array<double, kernelheight * kernelwidth> kernel)
{
    std::reverse(kernel.begin(), kernel.end());
    std::unique_ptr<uint8_t[]> buf = std::make_unique<uint8_t[]>(kernelheight * kernelwidth);
    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;


    for (int i = kernelheightradius; i < height - kernelheightradius; ++i)
    {
        for (int j = kernelwidthradius; j < width - kernelwidthradius; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                int pos = (i * width + j) * 3 + k;
                int shift = (pos - kernelwidthradius * 3) + width * 3 * kernelheightradius;
                int index = 0;

                for (int l = 0; l < kernelheight; ++l, shift -= 3 * width)
                {
                    for (int m = 0; m < kernelwidth; ++m, ++index)
                    {
                        buf[index] = img_in[shift + m * 3];
                    }
                }

                img_out[pos] = static_cast<uint8_t>(kernelcalc<kernelheight, kernelwidth>(buf.get(), kernel));
            }
        }
    }

    borderhandling<kernelheight, kernelwidth>(img_in, width, height, img_out, kernel);
}

template <unsigned kernelheight, unsigned kernelwidth>
void borderhandling(const uint8_t *img_in, size_t width, size_t height, uint8_t *img_out, std::array<double, kernelheight * kernelwidth> kernel)
{
    std::unique_ptr<uint8_t[]> buf = std::make_unique<uint8_t[]>(kernelheight * kernelwidth);
    std::array<std::pair<int, int>, kernelheight * kernelwidth> indicies;

    int starty = -static_cast<int>((kernelheight + 1) / 2 - 1);
    int endy = (kernelheight + 1) / 2 - 1;

    int startx = -static_cast<int>((kernelwidth + 1) / 2 - 1);
    int endx = (kernelwidth + 1) / 2 - 1;

    int pos = 0;
    int counter = 0;

    int kernelwidthradius = (kernelwidth - 1) / 2;
    int kernelheightradius = (kernelheight - 1) / 2;

    for (int i = 0; i < kernelheightradius; ++i){
        for (int j = 0; j < width; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirror(i + k, j + l, height - 1, width - 1);
                    
                    if(i == 11 && j ==0){
//                    std::cout << "First: " << indicies.at(pos).first << "   " << " Second: " << indicies.at(pos).second << std::endl;
                    //std::cout << "i + k: " << i + k << "   " << " j + l: " << j + l << std::endl;
                    }
                    
                }
            }
            for (int m = 0; m < 3; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buf[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * 3 + m];
                    if(i == 11 && j == 0 && m == 0){
//                    std::cout << "T: " << (indicies.at(n).first * width + indicies.at(n).second) * 3 + m << std::endl;
                    
                    }
                }

                img_out[(i * width + j) * 3 + m] = static_cast<uint8_t>(kernelcalc<kernelheight, kernelwidth>(buf.get(), kernel));
            }
        }
    }
    
    for(int i = (height - kernelheightradius); i < height; ++i){
        for(int j = 0; j < width; ++j,  pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirror(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < 3; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buf[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * 3 + m];
                }
                img_out[(i * width + j) * 3 + m] = static_cast<uint8_t>(kernelcalc<kernelheight, kernelwidth>(buf.get(), kernel));
            }
        }
    }

    for(int i = kernelheightradius; i < (height - kernelheightradius); ++i){
        for(int j = 0; j < kernelwidthradius; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirror(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < 3; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buf[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * 3 + m];
                }
                img_out[(i * width + j) * 3 + m] = static_cast<uint8_t>(kernelcalc<kernelheight, kernelwidth>(buf.get(), kernel));
            }
        }
    }

    for(int i = kernelheightradius; i < (height - kernelheightradius); ++i){
        for(int j = width - kernelwidthradius; j < width; ++j, pos = 0){
            for (int k = starty; k <= endy; ++k){
                for (int l = startx; l <= endx; ++l, ++pos){
                    indicies.at(pos) = mirror(i + k, j + l, height - 1, width - 1);
                }
            }

            for (int m = 0; m < 3; ++m){
                for (int n = 0; n < indicies.size(); ++n){
                    buf[n] = img_in[(indicies.at(n).first * width + indicies.at(n).second) * 3 + m];
                }
                img_out[(i * width + j) * 3 + m] = static_cast<uint8_t>(kernelcalc<kernelheight, kernelwidth>(buf.get(), kernel));
            }
        }
    }
        
}
constexpr std::pair<int, int> mirror(int posy, int posx, int endy, int endx)
{
    if ((posx >= 0 && posx <= endx) && (posy >= 0 && posy <= endy))
    {
        return std::make_pair(posy, posx);
    }
    int distancex = posx < 0 ? -posx : posx;
    int repx = distancex / endx;
    int modx = distancex % endx;
    int index = repx % 2 == 0 ? modx : endx - modx;

    int distancey = posy < 0 ? -posy : posy;
    int repy = distancey / endy;
    int mody = distancey % endy;
    int indey = repy % 2 == 0 ? mody : endy - mody;

    return std::make_pair(indey, index);
}

template <unsigned kernelheight, unsigned kernelwidth, unsigned sigma>
constexpr std::array<double, kernelheight * kernelwidth> gaussian_filter()
{

    std::array<double, kernelheight * kernelwidth> kernel;
    double sum = 0.0;
    double tmp = 0.0;
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

constexpr double hg(int i, int j, int meani, int meanj, int sigma)
{
    return std::exp(-0.5 * ((i - meani) * (i - meani) + (j - meanj) * (j - meanj)) / (sigma * sigma));
}
