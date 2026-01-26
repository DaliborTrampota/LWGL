#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>

using Data = unsigned char*;

constexpr const char* getFileName(const char* path) {
    const char* file = path;
    while (*path) {
        if (*path == '/' || *path == '\\') {
            file = path + 1;
        }
        ++path;
    }
    return file;
}

#define PRINT_ERROR(message) printError(getFileName(__FILE__), __LINE__, message)

#define ASSERT(condition) \
    if (!(condition)) { \
        PRINT_ERROR("Assertion failed: " #condition); \
    }


inline void printError(const char* file, int line, const std::string& message) {
    std::cout << file << ":" << line << " " << message << std::endl;
}


inline Data generateData(int width, int height, int channels = 4) {
    Data data = new unsigned char[width * height * channels];
    for (size_t i = 0; i < width * height * channels; i++) {
        data[i] = (unsigned char)rand() % 256;
    }
    return data;
}

inline bool dataEquals(Data data1, Data data2, int width, int height, int channels = 4) {
    for (size_t i = 0; i < width * height * channels; i++) {
        if (data1[i] != data2[i])
            return false;
    }
    return true;
}
