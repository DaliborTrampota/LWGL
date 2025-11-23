#include "LWGL/texture/ImageData.h"

#include <glad/glad.h>
#include <format>
#include <stdexcept>


using namespace gl;

ImageData::ImageData(const char* path) {
    data = stbi_load(path, &width, &height, &channels, 0);
    if (!data)
        throw std::runtime_error(std::format("Failed to load image: {}", stbi_failure_reason()));

    this->path = path;

    switch (channels) {
        case 1: format = ImageFormat::GRAY; break;
        case 2: format = ImageFormat::GRAY_ALPHA; break;
        case 3: format = ImageFormat::RGB; break;
        case 4: format = ImageFormat::RGBA; break;
        default: throw std::runtime_error("Unsupported number of channels");
    }
}

ImageData::~ImageData() {
    stbi_image_free(data);
}

ImageData::ImageData(ImageData&& other) noexcept
    : width(other.width),
      height(other.height),
      channels(other.channels),
      data(other.data),
      format(other.format),
      path(other.path) {
    other.data = nullptr;  // Take ownership
}

ImageData& ImageData::operator=(ImageData&& other) noexcept {
    if (this != &other) {
        stbi_image_free(data);  // free existing data

        width = other.width;
        height = other.height;
        channels = other.channels;
        data = other.data;
        format = other.format;
        path = other.path;

        other.data = nullptr;  // take ownership
    }
    return *this;
}