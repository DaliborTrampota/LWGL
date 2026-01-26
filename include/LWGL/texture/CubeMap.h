#pragma once

#include "TextureBase.h"

namespace gl {

    class CubeMap : public TextureBase {
      public:
        CubeMap(bool immutable = true) : TextureBase(TextureType::CubeMap, immutable) {};

        /// @note The textures are in the order of CubeFace enum: Right, Left, Top, Bottom, Front, Back
        static CubeMap fromImageData(
            ImageData imageData[6], TextureParams params, bool immutable = true
        );
        // static Texture2D forRenderTarget(TextureStorage storage, TextureParams params);

        void create(TextureParams params);

        /// @note This is not needed for mutable cube maps, it does nothing
        void allocate(TextureStorage storage);
        void upload(CubeFace face, int width, ImageFormat format, Data data);
        void upload(CubeFace face, const ImageData& imageData);
        void upload(CubeFace face, const RawImageData& rawImageData);

        int width() { return m_width; }

      private:
        int m_width = 0;
        int m_channels = 0;
    };
}  // namespace gl