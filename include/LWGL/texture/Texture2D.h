#pragma once

#include "TextureBase.h"

namespace gl {
    class Texture2D : public TextureBase {
      public:
        Texture2D(bool immutable = true) : TextureBase(TextureType::Texture2D, immutable) {};

        static Texture2D fromImageData(
            const ImageData& imageData, TextureParams params, bool immutable = true
        );
        static Texture2D fromRawData(
            const RawImageData& rawImageData, TextureParams params, bool immutable = true
        );
        static Texture2D forRenderTarget(
            TextureStorage storage, TextureParams params, bool immutable = true
        );

        void create(TextureParams params);
        void allocate(TextureStorage storage);
        void upload(ImageFormat format, Data data);

        int width() const { return m_width; }
        int height() const { return m_height; }
        int channels() const { return m_channels; }

      private:
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
    };
}  // namespace gl