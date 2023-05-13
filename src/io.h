#pragma once
#include <string>
#include <vector>
#include <iostream>

struct IOhandler
{
    float r = 0.3f;
    float g = 0.59f;
    float b = 0.11f;

    bool seperate = true;
    int ksize = 3;
    int border = 1;

    int sobelkSize = 5;
    int gausskSize = 3;
    int threshold = 45;
    bool gradient = false;

    std::vector<std::string> splitstring(std::string string, std::string delimiter);
    bool isFloat(std::string string);
    bool isInt(std::string string);

    bool pathHandler(std::string input);
    
    bool grayHandler(std::string input);
    bool gaussianHandler(std::string input);
    bool sobelHandler(std::string input);
    
};