#include "FileIO.h"

std::unique_ptr<uint8_t> readFile(const char* path){
    FILE* file = std::fopen(path, "r");
    if(!file) {
        std::perror("File opening failed");
        return nullptr;
    }

    if (access(path, R_OK))
    {
        std::perror("File reading failed");
        fclose(file);
        return nullptr;
    }

    struct stat s;
    if(stat(path, &s) < 0){
        std::perror("Stat error");
        return nullptr;
    }

    uint8_t* rawfile = new uint8_t[s.st_size];
    fread(rawfile, sizeof(char) , s.st_size, file);
    fclose(file);

    return std::unique_ptr<uint8_t>(rawfile);;
    
}
