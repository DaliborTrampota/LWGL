#pragma once

#include <cstdint>
#include <stdexcept>
#include "ImageData.h"

namespace gl {

    struct Settings {
        enum Wrap {
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
        };
        enum Filter {
            Linear,
            Nearest,
        };

        Settings(Wrap wrap, Filter filter)
            : wrapS(wrap),
              wrapT(wrap),
              wrapR(wrap),
              minFilter(filter),
              magFilter(filter) {}

        Settings(Wrap wrapS, Wrap wrapT, Wrap wrapR, Filter minFilter, Filter magFilter)
            : wrapS(wrapS),
              wrapT(wrapT),
              wrapR(wrapR),
              minFilter(minFilter),
              magFilter(magFilter) {}

        Wrap wrapS = MirroredRepeat;
        Wrap wrapT = MirroredRepeat;
        Wrap wrapR = MirroredRepeat;  // Only used for 3D textures
        Filter minFilter = Linear;
        Filter magFilter = Linear;

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
                ImageFormat::Depth,
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

    // enum class FBOAttachment : uint8_t {
    //     Depth,
    //     Stencil,
    //     DepthStencil,
    //     Color,
    // };

    // inline FBOAttachment operator>>(FBOAttachment attachment, int offset) {
    //     return static_cast<FBOAttachment>(static_cast<int>(attachment) + offset);
    // }

    // inline uint8_t operator+(FBOAttachment attachment, int offset) {
    //     return static_cast<int>(attachment) + offset;
    // }

    namespace FBOAttachment {
        using Attachment = uint8_t;
        constexpr Attachment Depth = 0;
        constexpr Attachment Stencil = 1;
        constexpr Attachment DepthStencil = 2;
        constexpr Attachment Color = 3;

        // Helper function to create color attachment indices
        // Usage: FBOAttachmentN::ColorN(0), FBOAttachmentN::ColorN(1), etc.
        constexpr Attachment ColorN(uint8_t offset) {
            return Color + offset;
        }
    };  // namespace FBOAttachment


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