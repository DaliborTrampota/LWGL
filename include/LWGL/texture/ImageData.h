#pragma once

#include <tools/stb_image.h>
#include <string>

#include "LWGL/GLTypes.h"

namespace gl {
    /// @brief The format of the image (eg. image channels)
    /// @note Image format should be one of the following values:
    /// GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
    /// @note More info in OpenGL documentation https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    // using ImageFormat = GLenum;

    /// @brief The type of the image data (eg. pixel representation)
    /// @note The valid enum values can be found in the OpenGL documentation https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    // using ImageDataType = GLenum;

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

    // TODO support for 3d images? research if such formats exist and how to load them?
    struct ImageData {
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* data = nullptr;
        ImageFormat format = ImageFormat::RGBA;
        ImageDataType dataType = ImageDataType::UChar;
        std::string path;

        ImageData(const char* path);
        ImageData(
            unsigned char* data, int w, int h, int ch, ImageFormat format, ImageDataType dataType
        );
        ~ImageData();

        ImageData& operator=(const ImageData&) = delete;
        ImageData(const ImageData&) = delete;
        ImageData(ImageData&& other) noexcept;
        ImageData& operator=(ImageData&& other) noexcept;
    };

    struct RawImageData {
        int width = 0;
        int height = 0;
        int depth = 0;
        int channels = 0;
        void* data = nullptr;

        GLenum format;
        GLenum internalFormat;
        GLenum dataType;

        RawImageData(
            void* data,
            int w,
            int h,
            int d,
            int ch,
            GLenum format,
            GLenum dataType,
            GLenum internalFormat = GL_NONE
        );
    };
}  // namespace gl