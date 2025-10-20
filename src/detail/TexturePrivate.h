#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/TextureBase.h"

#include <glad/glad.h>

namespace gl::detail {
    constexpr GLenum toGLWrap(Settings::Option opt) {
        switch (opt) {
            case Settings::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case Settings::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case Settings::ClampToBorder: return GL_CLAMP_TO_BORDER;
            default: return GL_MIRRORED_REPEAT;
        }
    }

    constexpr GLenum toGLFilter(Settings::Option opt) {
        switch (opt) {
            case Settings::Linear: return GL_LINEAR;
            case Settings::Nearest: return GL_NEAREST;
            default: return GL_LINEAR;
        }
    }

    constexpr GLenum toGLType(TextureType type) {
        switch (type) {
            case TextureType::Texture2D: return GL_TEXTURE_2D;
            case TextureType::Texture3D: return GL_TEXTURE_3D;
            case TextureType::TextureArray: return GL_TEXTURE_2D_ARRAY;
            case TextureType::Texture1D: return GL_TEXTURE_1D;
            case TextureType::TextureArray1D: return GL_TEXTURE_1D_ARRAY;
            case TextureType::CubeMap: return GL_TEXTURE_CUBE_MAP;
            case TextureType::CubeMapArray: return GL_TEXTURE_CUBE_MAP_ARRAY;
            case TextureType::Multisample: return GL_TEXTURE_2D_MULTISAMPLE;
            case TextureType::MultisampleArray: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
            case TextureType::Buffer: return GL_TEXTURE_BUFFER;
        }
        return GL_NONE;
    }

    constexpr GLenum toGLFormat(gl::ImageFormat format) {
        switch (format) {
            case gl::ImageFormat::RGB: return GL_RGB;
            case gl::ImageFormat::RGBA: return GL_RGBA;
            case gl::ImageFormat::GRAY: return GL_RED;
            case gl::ImageFormat::GRAY_ALPHA: return GL_RG;
            case gl::ImageFormat::DEPTH: return GL_DEPTH_COMPONENT;
            case gl::ImageFormat::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
            case gl::ImageFormat::STENCIL: return GL_STENCIL_INDEX;
        }
        return GL_NONE;  // Should not happen
    }

    constexpr GLenum toGLInternalFormat(gl::ImageFormat format) {
        switch (format) {
            case gl::ImageFormat::RGB: return GL_RGB8;
            case gl::ImageFormat::RGBA: return GL_RGBA8;
            case gl::ImageFormat::GRAY: return GL_R8;
            case gl::ImageFormat::GRAY_ALPHA: return GL_RG8;
            case gl::ImageFormat::DEPTH: return GL_DEPTH_COMPONENT;
            case gl::ImageFormat::DEPTH_STENCIL:
                return GL_DEPTH_STENCIL;
                //case gl::ImageFormat::STENCIL: return GL_STENCIL_INDEX8; // ?
        }
        return GL_NONE;  // Should not happen
    }

    constexpr GLenum toGLCubeFace(CubeFace face) {
        switch (face) {
            case CubeFace::Right: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case CubeFace::Left: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case CubeFace::Top: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case CubeFace::Bottom: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case CubeFace::Back: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            case CubeFace::Front: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        }
        return GL_NONE;  // Should not happen
    }

    constexpr GLenum toGLDataType(ImageDataType type) {
        switch (type) {
            case ImageDataType::UChar: return GL_UNSIGNED_BYTE;
            case ImageDataType::UShort: return GL_UNSIGNED_SHORT;
            case ImageDataType::UInt: return GL_UNSIGNED_INT;
            case ImageDataType::Char: return GL_BYTE;
            case ImageDataType::Short: return GL_SHORT;
            case ImageDataType::Int: return GL_INT;
            case ImageDataType::HalfFloat: return GL_HALF_FLOAT;
            case ImageDataType::Float: return GL_FLOAT;
        }
        return GL_NONE;  // Should not happen
    }

    constexpr GLenum toGLAttachmentType(FBOAttachment attachment) {
        switch (attachment) {
            case FBOAttachment::Depth: return GL_DEPTH_ATTACHMENT;
            //case FBOAttachment::Stencil: return GL_STENCIL_ATTACHMENT;
            case FBOAttachment::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
            case FBOAttachment::Color: return GL_COLOR_ATTACHMENT0;
            default:
                uint8_t idx = static_cast<uint8_t>(attachment);
                if (idx >= 32)
                    throw std::runtime_error("Color attachment index out of bounds");

                return GL_COLOR_ATTACHMENT0 + (idx - static_cast<int>(FBOAttachment::Color));
        }
    }

    void ConfigureTexture(GLenum type, const Settings& settings);
    void Data1D(GLenum type, int width, gl::ImageFormat format, unsigned char* data);
    void Data2D(
        GLenum type,
        int width,
        int height,
        gl::ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );
    void Data3D(
        GLenum type, int width, int height, int depth, gl::ImageFormat format, unsigned char* data
    );
    void SubData3D(
        GLenum type,
        int layer,
        int width,
        int height,
        int depth,
        gl::ImageFormat format,
        unsigned char* data
    );
}  // namespace gl::detail
