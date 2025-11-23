#pragma once

#include <tools/stb_image.h>
#include <string>

#include "LWGL/GLTypes.h"

namespace gl {

    enum class ImageFormat {
        Unknown,
        RGB,           // Red, Green, Blue
        RGBA,          // Red, Green, Blue, Alpha
        Gray,          // Grayscale
        GrayAlpha,     // Grayscale with Alpha
        Depth,         // Depth
        DepthStencil,  // Depth and Stencil
        Stencil,       // Stencil
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

    struct RawImageData {
        int width = 0;
        int height = 0;
        int channels = 0;
        void* data = nullptr;

        GLenum format;
        GLenum internalFormat;
        GLenum dataType;

        RawImageData(
            void* data,
            int w,
            int h,
            int ch,
            GLenum format,
            GLenum dataType,
            GLenum internalFormat = GL_NONE
        );
    };
}  // namespace gl