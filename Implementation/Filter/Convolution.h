#include <cmath>
#include <array>
#include <iostream>

float hg(int i, int j, int meani, int meanj, int sigma){
    return exp(-0.5 *( pow(i - meani, 2.0) + pow(j - meanj, 2.0))/ (sigma * sigma));
}

template<unsigned M, unsigned N, unsigned sigma>
std::array<std::array<float, N>, M>  gaussian_filter(){

    std::array<std::array<float, N>, M> kernel;
    float sum = 0;
    float tmp = 0;
    int meani = M /2;
    int meanj = N /2;

    for(int i = 0; i < M; ++i){
        for(int j = 0; j < N; ++j){
            tmp = hg(i, j, meani, meanj, sigma);
            sum += tmp; 
            kernel.at(i).at(j) = tmp;
        }
    }
    
    for(int i = 0; i < M; ++i){
        for(int j = 0; j < N; ++j){
            kernel.at(i).at(j) /= sum;
        }
    }
    
    return kernel;
}

void convolution(const uint8_t* img_in, size_t width, size_t height, uint8_t* img_out, std::vector<float> kernel);


