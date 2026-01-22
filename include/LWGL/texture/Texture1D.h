#pragma once

#include "TextureBase.h"

namespace gl {
    class Texture1D : public TextureBase {
      public:
        Texture1D() : TextureBase(TextureType::Texture1D) {};

        void create(Settings settings);
        void load(const gl::ImageData& imageData);
        void load(const gl::RawImageData& rawImageData);
        void loadRaw(int w, int ch, ImageFormat format, Data data);

      private:
        int m_width = 0;
        int m_channels = 0;
    };
}  // namespace gl