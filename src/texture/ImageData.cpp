#include "LWGL/texture/ImageData.h"

#include <glad/glad.h>
#include <format>
#include <stdexcept>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace gl;

ImageData::ImageData(const char* path, int forceChannels) {
    if (forceChannels < 0 || forceChannels > 4) {
        this->path = std::format(
            "Failed to load image: invalid forceChannels {} (expected 0-4)", forceChannels
        );
        return;
    }

    data = stbi_load(path, &width, &height, &channels, forceChannels);
    if (!data) {
        this->path = std::format("Failed to load image: {}", stbi_failure_reason());
        return;
    }

    this->path = path;
    if (forceChannels != 0)
        channels = forceChannels;
    switch (channels) {
        case 1: format = ImageFormat::Gray; break;
        case 2: format = ImageFormat::GrayAlpha; break;
        case 3: format = ImageFormat::RGB; break;
        case 4: format = ImageFormat::RGBA; break;
        default:
            stbi_image_free(data);
            data = nullptr;
            this->path = std::format(
                "Failed to load image: unsupported channel count {} (expected 1-4)", channels
            );
            return;
    }
}

ImageData::ImageData(
    unsigned char* data, int w, int h, int ch, ImageFormat format, ImageDataType dataType
)
    : width(w),
      height(h),
      channels(ch),
      data(data),
      format(format),
      dataType(dataType) {}

ImageData::~ImageData() {
    if (data)
        stbi_image_free(data);
}

ImageData::ImageData(ImageData&& other) noexcept
    : width(other.width),
      height(other.height),
      channels(other.channels),
      data(other.data),
      format(other.format),
      dataType(other.dataType),
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
        dataType = other.dataType;
        path = other.path;

        other.data = nullptr;  // take ownership
    }
    return *this;
}

ImageData ImageData::resize(int w, int h, ImageResizeFilter filter) const {
    stbir_pixel_layout pixelLayout;

    switch (channels) {
        case 1: pixelLayout = STBIR_1CHANNEL; break;
        case 2: pixelLayout = STBIR_2CHANNEL; break;
        case 3: pixelLayout = STBIR_RGB; break;
        case 4: pixelLayout = STBIR_RGBA; break;
        default: throw std::runtime_error("Invalid channel count");
    }

    stbir_datatype type;
    size_t bytesPerChannel = 0;
    switch (dataType) {
        case ImageDataType::UChar:
            type = STBIR_TYPE_UINT8;
            bytesPerChannel = 1;
            break;
        case ImageDataType::UShort:
            type = STBIR_TYPE_UINT16;
            bytesPerChannel = 2;
            break;
        case ImageDataType::HalfFloat:
            type = STBIR_TYPE_HALF_FLOAT;
            bytesPerChannel = 2;
            break;
        case ImageDataType::Float:
            type = STBIR_TYPE_FLOAT;
            bytesPerChannel = 4;
            break;
        case ImageDataType::Char:
        case ImageDataType::Short:
        case ImageDataType::Int:
        case ImageDataType::UInt:
            throw std::runtime_error("ImageData::resize: unsupported datatype");
    }
    // TODO for 8-bit color textures the gamma-correct choice is STBIR_TYPE_UINT8_SRGB rather than STBIR_TYPE_UINT8, otherwise smoothing resamples in gamma space and darkens slightly.
    unsigned char* out = static_cast<unsigned char*>(
        malloc(static_cast<size_t>(w) * h * channels * bytesPerChannel)
    );
    stbir_resize(
        data,
        width,
        height,
        0,  // in stride - 0 stb will figure out
        out,
        w,
        h,
        0,  // out stride
        pixelLayout,
        type,
        STBIR_EDGE_CLAMP,
        filter == ImageResizeFilter::Linear ? STBIR_FILTER_TRIANGLE : STBIR_FILTER_POINT_SAMPLE
    );
    ImageData data(out, w, h, channels, format, dataType);
    data.path = path + " (resized)";
    return data;
}

RawImageData::RawImageData(
    void* data, int w, int h, int d, int ch, GLenum format, GLenum dataType, GLenum internalFormat
)
    : width(w),
      height(h),
      depth(d),
      channels(ch),
      data(data),
      format(format),
      internalFormat(internalFormat),
      dataType(dataType) {}
