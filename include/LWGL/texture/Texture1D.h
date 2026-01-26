#pragma once

#include "TextureBase.h"

namespace gl {
    class Texture1D : public TextureBase {
      public:
        Texture1D(bool immutable = true) : TextureBase(TextureType::Texture1D, immutable) {};

        static Texture1D fromImageData(
            const ImageData& imageData, TextureParams params, bool immutable = true
        );
        static Texture1D fromRawData(
            const RawImageData& rawImageData, TextureParams params, bool immutable = true
        );
        static Texture1D forRenderTarget(
            TextureStorage storage, TextureParams params, bool immutable = true
        );

        void create(TextureParams params);
        void allocate(TextureStorage storage);
        void upload(ImageFormat format, Data data);

        int width() const { return m_width; }
        int channels() const { return m_channels; }

      private:
        int m_width = 0;
        int m_channels = 0;
    };
}  // namespace gl