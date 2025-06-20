#pragma once

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
        int layers;
        int width;
        int height;
        ImageFormat format = ImageFormat::RGBA;

        static ArraySettings Pixelated() {
            return {
                MirroredRepeat,
                MirroredRepeat,
                MirroredRepeat,
                Nearest,
                Nearest,
            };
        }
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