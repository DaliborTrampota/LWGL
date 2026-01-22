#pragma once

#include <glad/glad.h>
#include <stdexcept>

#include "LWGL/buffer/VertexLayout.h"
#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/TextureBase.h"


namespace gl::detail {

    constexpr GLsizei sizeOfType(GLenum type) {
        switch (type) {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
            case GL_SHORT:
            case GL_UNSIGNED_SHORT: return sizeof(GLshort);
            case GL_INT_2_10_10_10_REV:
            case GL_INT:
            case GL_UNSIGNED_INT_2_10_10_10_REV:
            case GL_UNSIGNED_INT: return sizeof(GLint);
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_DOUBLE: return sizeof(GLdouble);
            case GL_HALF_FLOAT: return sizeof(GLhalf);
            default: throw std::runtime_error("Unknown GL type");
        }
    }

    constexpr int getType(VertexAttribute::Type type) {
        switch (type) {
            case VertexAttribute::Float: return GL_FLOAT;
            case VertexAttribute::Int: return GL_INT;
            case VertexAttribute::UInt: return GL_UNSIGNED_INT;
            // case gl::VertexAttribute::Byte: return GL_BYTE;
            // case gl::VertexAttribute::UByte: return GL_UNSIGNED_BYTE;
            // case gl::VertexAttribute::Short: return GL_SHORT;
            // case gl::VertexAttribute::UShort: return GL_UNSIGNED_SHORT;
            // case gl::VertexAttribute::Double: return GL_DOUBLE;
            default: return GL_FLOAT;
        }
    }

    // Texture settings
    constexpr GLenum toGLWrap(Settings::Wrap opt) {
        switch (opt) {
            case Settings::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case Settings::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case Settings::ClampToBorder: return GL_CLAMP_TO_BORDER;
        }
    }

    constexpr GLenum toGLFilter(Settings::Filter opt) {
        switch (opt) {
            case Settings::Linear: return GL_LINEAR;
            case Settings::Nearest: return GL_NEAREST;
        }
    }

    // Formats
    constexpr GLenum toGLFormat(ImageFormat format) {
        switch (format) {
            case ImageFormat::RGB: return GL_RGB;
            case ImageFormat::RGBA: return GL_RGBA;
            case ImageFormat::Gray: return GL_RED;
            case ImageFormat::GrayAlpha: return GL_RG;
            case ImageFormat::Depth: return GL_DEPTH_COMPONENT;
            case ImageFormat::DepthStencil: return GL_DEPTH_STENCIL;
            case ImageFormat::Stencil: return GL_STENCIL_INDEX;
            case ImageFormat::Unknown: {
                throw std::runtime_error("Invalid image format");
            }
        }
    }


    constexpr GLenum toGLInternalFormat(ImageFormat format) {
        switch (format) {
            case ImageFormat::RGB: return GL_RGB8;
            case ImageFormat::RGBA: return GL_RGBA8;
            case ImageFormat::Gray: return GL_R8;
            case ImageFormat::GrayAlpha: return GL_RG8;
            case ImageFormat::Depth: return GL_DEPTH_COMPONENT24;
            case ImageFormat::DepthStencil: return GL_DEPTH24_STENCIL8;
            case ImageFormat::Stencil: return GL_STENCIL_INDEX8;
            case ImageFormat::Unknown: {
                throw std::runtime_error("Invalid image format");
            }
        }
    }

    constexpr GLenum toGLInternalFormat(ImageFormat format, ImageDataType dataType) {
        // Handle float precision formats
        if (dataType == ImageDataType::Float || dataType == ImageDataType::HalfFloat) {
            switch (format) {
                case ImageFormat::Gray: return dataType == ImageDataType::Float ? GL_R32F : GL_R16F;
                case ImageFormat::GrayAlpha:
                    return dataType == ImageDataType::Float ? GL_RG32F : GL_RG16F;
                case ImageFormat::RGB:
                    return dataType == ImageDataType::Float ? GL_RGB32F : GL_RGB16F;
                case ImageFormat::RGBA:
                    return dataType == ImageDataType::Float ? GL_RGBA32F : GL_RGBA16F;
                case ImageFormat::Depth: return GL_DEPTH_COMPONENT32F;  // use 16F?
                default: break;
            }
        }
        // Fall back to regular internal format
        return toGLInternalFormat(format);
    }
    // Textures
    constexpr GLenum toGLCubeFace(CubeFace face) {
        switch (face) {
            case CubeFace::Right: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case CubeFace::Left: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case CubeFace::Top: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case CubeFace::Bottom: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case CubeFace::Back: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
            case CubeFace::Front: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        }
    }

    constexpr GLenum toGLTexture(TextureType type) {
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

    constexpr GLenum toGLAttachmentType(FBOAttachment::Attachment attachment) {
        switch (attachment) {
            case FBOAttachment::Depth: return GL_DEPTH_ATTACHMENT;
            case FBOAttachment::Stencil: return GL_STENCIL_ATTACHMENT;
            case FBOAttachment::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
            case FBOAttachment::Color: return GL_COLOR_ATTACHMENT0;
            default:
                // For color attachments with offsets
                return GL_COLOR_ATTACHMENT0 + (attachment - FBOAttachment::Color);
        }
    }

}  // namespace gl::detail