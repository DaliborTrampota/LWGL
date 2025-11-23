#pragma once

#include <tools/stb_image.h>
#include <format>
#include <stdexcept>
#include <string>

namespace gl {

    enum class ImageFormat {
        Unknown,
        RGB,            // Red, Green, Blue
        RGBA,           // Red, Green, Blue, Alpha
        GRAY,           // Grayscale
        GRAY_ALPHA,     // Grayscale with Alpha
        DEPTH,          // Depth
        DEPTH_STENCIL,  // Depth and Stencil
        STENCIL,        // Stencil
    };

    enum class ImageDataType {
        UChar,  // unsigned byte
        UShort,
        UInt,

        Char,  // byte
        Short,
        Int,
        HalfFloat,
        Float,
    };

    struct ImageData {
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* data = nullptr;
        ImageFormat format = ImageFormat::RGBA;
        std::string path;

        ImageData(const char* path);
        ~ImageData();

        ImageData& operator=(const ImageData&) = delete;
        ImageData(const ImageData&) = delete;
        ImageData(ImageData&& other) noexcept;
        ImageData& operator=(ImageData&& other) noexcept;
    };
}  // namespace gl