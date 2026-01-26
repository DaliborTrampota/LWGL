#pragma once

#include "ImageData.h"

#include <glm/glm.hpp>


namespace gl {

    struct TextureParams {
        enum Wrap {
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
        };
        enum Filter {
            Linear,
            Nearest,
        };

        TextureParams(Wrap wrap, Filter filter)
            : wrapS(wrap),
              wrapT(wrap),
              wrapR(wrap),
              minFilter(filter),
              magFilter(filter) {}

        Wrap wrapS = MirroredRepeat;
        Wrap wrapT = MirroredRepeat;
        Wrap wrapR = MirroredRepeat;  // Only used for 3D textures
        Filter minFilter = Linear;
        Filter magFilter = Linear;
        float borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

        static TextureParams Pixelated() { return TextureParams(MirroredRepeat, Nearest); }
        static TextureParams Cubemap() { return TextureParams(ClampToEdge, Linear); }
        static TextureParams Depth() { return TextureParams(ClampToBorder, Nearest); }
    };

    struct TextureStorage {
        int width = 0;
        int height = 0;
        int depth = 0;
        ImageFormat format = ImageFormat::RGBA;
        ImageDataType dataType = ImageDataType::UChar;

        static TextureStorage FBODepth(int width, int height) {
            return {
                .width = width,
                .height = height,
                .format = ImageFormat::Depth,
                .dataType = ImageDataType::Float,
            };
        }
    };

    using UInt = unsigned int;
    using Data = unsigned char*;

    class TextureBase {
      public:
        TextureBase(TextureType type, bool immutable);
        virtual ~TextureBase();

        UInt id() const { return m_id; }
        void bind() const;
        void activate(UInt unit) const;
        void unbind(UInt unit) const;
        TextureType type() const { return m_type; }

        static void unbind(TextureType type);

      protected:
        UInt m_id = 0;
        bool m_immutable = true;
        TextureType m_type;  // OpenGL texture type (e.g., GL_TEXTURE_2D)
    };
}  // namespace gl