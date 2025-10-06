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
            case gl::ImageDataType::UShort: return GL_UNSIGNED_SHORT;
            case gl::ImageDataType::UInt: return GL_UNSIGNED_INT;
            case gl::ImageDataType::Char: return GL_BYTE;
            case gl::ImageDataType::Short: return GL_SHORT;
            case gl::ImageDataType::Int: return GL_INT;
            case gl::ImageDataType::HalfFloat: return GL_HALF_FLOAT;
            case gl::ImageDataType::Float: return GL_FLOAT;
        }
        return GL_NONE;  // Should not happen
    }

    constexpr GLenum toGLAttachmentType(FBOType type) {
        switch (type) {
            case FBOType::Depth: return GL_DEPTH_ATTACHMENT;
            case FBOType::Color: return GL_COLOR_ATTACHMENT0;
            case FBOType::Stencil: return GL_STENCIL_ATTACHMENT;
            case FBOType::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
        }
        return GL_NONE;  // Should not happen
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
