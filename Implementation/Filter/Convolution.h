#include "Mymath.h"
#include <cstdio>
#include <cinttypes>
#include <array>


namespace Convolution{

    template<unsigned M, unsigned N, unsigned sigma>
    constexpr std::array<float, M * N> gaussian_filter();

    constexpr float hg(int i, int j, int meani, int meanj, int sigma);

    template<unsigned kernelwidth, unsigned kernelheight, std::array<float, kernelwidth * kernelheight> kernel>
    class Convolution {
        void convolution(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out);  
    };


    template<unsigned M, unsigned N, unsigned sigma>
    constexpr std::array<float, M * N> gaussian_filter(){

        std::array<float, M * N> kernel;
        float sum = 0.0;
        float tmp = 0.0;
        int meani = M /2;
        int meanj = N /2;
        int pos = 0;

        for(int i = 0; i < M; ++i){
            for(int j = 0; j < N; ++j, ++pos){
                tmp = hg(i, j, meani, meanj, sigma);
                sum += tmp; 
                kernel.at(pos) = tmp;
            }
        }
        if(sum != 0.0){
            for(int i = 0; i < kernel.size(); ++i){
                kernel.at(i) /= sum;
            }
        }
        
        return kernel;
    }

    constexpr float hg(int i, int j, int meani, int meanj, int sigma){
        return mymath::exp(-0.5 * ((mymath::pow(i - meani, 2) + mymath::pow(j - meanj, 2)) / mymath::pow(sigma, 2)));
    }


    template<unsigned kernelheight, std::array<float, kernelheight> kernel>
    class Convolution<1, kernelheight, kernel> {
        void convolution(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){
            
        }
    };

    template<unsigned kernelwidth, std::array<float, kernelwidth> kernel>
    class Convolution<kernelwidth, 1, kernel> {
        void convolution(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out){

        }
    };



}

