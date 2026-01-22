#pragma once

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
        float borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

    struct RawArraySettings : public ArraySettings {
        GLenum format;
        GLenum dataType;
        GLenum internalFormat;
    };


    struct FrameBufferSettings : public Settings {
        int width;
        int height;
        ImageFormat format;
        ImageDataType dataType;

        static FrameBufferSettings Depth(int width, int height) {
            return {
                Settings(ClampToBorder, Nearest),
                width,
                height,
                ImageFormat::Depth,
                ImageDataType::Float,
            };
        };
    };


    using UInt = unsigned int;
    using Data = unsigned char*;

    class TextureBase {
      public:
        TextureBase(TextureType type);
        virtual ~TextureBase();

        UInt id() const { return m_id; }
        void bind() const;
        void activate(UInt unit) const;
        void unbind(UInt unit) const;
        TextureType type() const { return m_type; }

        static void unbind(TextureType type);

      protected:
        UInt m_id = 0;
        TextureType m_type;  // OpenGL texture type (e.g., GL_TEXTURE_2D)
    };
}  // namespace gl