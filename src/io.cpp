#include "io.h"

std::vector<std::string> IOhandler::splitstring(std::string string, std::string delimiter)
{
    std::vector<std::string> res;
    size_t pos = 0;
    std::string token;
    while ((pos = static_cast<size_t>(string.find(delimiter))) != std::string::npos)
    {
        token = string.substr(0, pos);
        res.emplace_back(token);
        string.erase(0, pos + delimiter.length());
    }
    res.emplace_back(string);

    return res;
}
bool IOhandler::isFloat(std::string string)
{
    if (string.empty())
        return false;

    char *p;
    strtof(string.c_str(), &p);
    return (*p) == '\0';
}

bool IOhandler::isInt(std::string string)
{
    if (string.empty())
        return false;

    char *p;
    strtol(string.c_str(), &p, 10);
    return (*p) == '\0';
}

bool IOhandler::pathHandler(std::string input){
    if(input.empty()){
        return false;
    }

    return input.find(".bmp") != std::string::npos;
}

bool IOhandler::grayHandler(std::string input)
{
    if (input.empty())
    {
        return false;
    }

    auto svec = splitstring(input, " ");

    if (svec.size() != 3)
    {
        return false;
    }
    for (size_t i = 0; i < svec.size(); ++i)
    {

        if (!isFloat(svec.at(i).c_str()))
        {
            return false;
        }
        float tmp = static_cast<float>(std::atof(svec.at(i).c_str()));

        if (tmp < 0.f || tmp > 1.f)
        {
            return false;
        }
        switch (i)
        {
        case 0:
        {
            r = tmp;
            break;
        }
        case 1:
        {
            g = tmp;
            break;
        }
        case 2:
        {
            b = tmp;
            break;
        }
        }
    }

    float delta = 1.f - (r + b + g);
    float absdelta = delta < 0 ? -delta : delta;
    if (absdelta > 0.01)
    {
        return false;
    }

    return true;
}

bool IOhandler::gaussianHandler(std::string input)
{
    if (input.empty())
    {
        return false;
    }

    auto svec = splitstring(input, " ");
    if (svec.size() != 3)
    {
        return false;
    }

    for (size_t i = 0; i < svec.size(); ++i)
    {
        if (!isInt(svec.at(i).c_str()))
        {
            return false;
        }
        int tmp = std::atoi(svec.at(i).c_str());

        switch (i)
        {
        case 0:
        {
            seperate = tmp;
            break;
        }
        case 1:
        {
            if (tmp < 0 || tmp % 2 == 0)
            {
                return false;
            }
            ksize = tmp;
            break;
        }
        case 2:
        {
            if (tmp < 0 || tmp > 3)
            {
                return false;
            }
            border = tmp;
            break;
        }
        }
    }

    return true;
}
bool IOhandler::sobelHandler(std::string input)
{
    if (input.empty())
    {
        return false;
    }

    auto svec = splitstring(input, " ");
    if (svec.size() != 4)
    {
        return false;
    }

    for (size_t i = 0; i < svec.size(); ++i)
    {
        if (!isInt(svec.at(i).c_str()))
        {
            return false;
        }
        int tmp = std::atoi(svec.at(i).c_str());

        switch (i)
        {
        case 0:
        {
            if (tmp < 3 || tmp % 2 == 0)
            {
                return false;
            }
            sobelkSize = tmp;
            break;
        }
        case 1:
        {
            if (tmp < 0 || tmp % 2 == 0)
            {
                return false;
            }
            gausskSize = tmp;
            break;
        }
        case 2:
        {
            if (tmp < 0 || tmp > 255)
            {
                return false;
            }
            threshold = tmp;
            break;
        }
        case 3:
        {
            if (tmp < 0 || tmp > 3)
            {
                return false;
            }
            gradient = tmp;
            break;
        }
        }
    }
    return true;
}

