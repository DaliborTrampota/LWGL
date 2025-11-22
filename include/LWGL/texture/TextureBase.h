#pragma once

#include <cstdint>
#include "ImageData.h"

namespace gl {

    struct Settings {
        enum Option {
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
            Linear,
            Nearest,
        };

        Settings(Option wrap, Option filter)
            : wrapS(wrap),
              wrapT(wrap),
              wrapR(wrap),
              minFilter(filter),
              magFilter(filter) {}

        Settings(Option wrapS, Option wrapT, Option wrapR, Option minFilter, Option magFilter)
            : wrapS(wrapS),
              wrapT(wrapT),
              wrapR(wrapR),
              minFilter(minFilter),
              magFilter(magFilter) {}

        Option wrapS = MirroredRepeat;
        Option wrapT = MirroredRepeat;
        Option wrapR = MirroredRepeat;  // Only used for 3D textures
        Option minFilter = Linear;
        Option magFilter = Linear;

        static Settings Pixelated() {
            return {
                MirroredRepeat,
                MirroredRepeat,
                MirroredRepeat,
                Nearest,
                Nearest,
            };
        }

        static Settings LinearClampToEdge() {
            return {
                ClampToEdge,
                ClampToEdge,
                ClampToEdge,
                Linear,
                Linear,
            };
        }

        static Settings Cubemap() {
            return {
                ClampToEdge,
                ClampToEdge,
                ClampToEdge,
                Linear,
                Linear,
            };
        }
    };

    struct ArraySettings : public Settings {
        unsigned layers;
        int width;
        int height;
        ImageFormat format = ImageFormat::RGBA;

        static ArraySettings Pixelated() { return {Settings(MirroredRepeat, Nearest)}; }
    };


    struct FrameBufferSettings : public Settings {
        int width;
        int height;
        ImageFormat format;
        ImageDataType dataType;

        static FrameBufferSettings Depth(int width, int height) {
            return {
                Settings(ClampToEdge, Nearest),
                width,
                height,
                ImageFormat::DEPTH,
                ImageDataType::Float,
            };
        };
    };

    enum class TextureType {
        Texture1D,
        Texture2D,
        Texture3D,

        TextureArray1D,
        TextureArray,

        CubeMap,
        CubeMapArray,

        Multisample,
        MultisampleArray,

        Buffer
    };


    enum class CubeFace {
        Right = 0,
        Left,
        Top,
        Bottom,
        Back,
        Front
    };

    enum class FBOAttachment : uint8_t {
        Depth,
        Stencil,
        DepthStencil,
        Color,
    };


    inline FBOAttachment operator>>(FBOAttachment attachment, int offset) {
        return static_cast<FBOAttachment>(static_cast<int>(attachment) + offset);
    }

    inline uint8_t operator+(FBOAttachment attachment, int offset) {
        return static_cast<int>(attachment) + offset;
    }


    using UInt = unsigned int;
    using Data = unsigned char*;

    class TextureBase {
      public:
        TextureBase(UInt unit, TextureType type);
        virtual ~TextureBase();

        UInt id() const { return m_id; }
        void bind() const;
        void unbind() const;

      protected:
        UInt m_id = 0;
        UInt m_unit = 0;
        TextureType m_type;  // OpenGL texture type (e.g., GL_TEXTURE_2D)
    };
}  // namespace gl