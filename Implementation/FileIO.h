#include <memory>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

std::unique_ptr<uint8_t[]> readFile(const char* path);
