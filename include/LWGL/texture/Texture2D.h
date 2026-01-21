#pragma once

#include "TextureBase.h"

namespace gl {
    class Texture2D : public TextureBase {
      public:
        Texture2D() : TextureBase(TextureType::Texture2D) {};

        void create(Settings settings);
        void load(const gl::ImageData& imageData);
        void load(const gl::RawImageData& rawImageData);
        void loadRaw(int w, int h, int ch, ImageFormat format, Data data);

        void update(ImageFormat format, Data data);

        int width() const { return m_width; }
        int height() const { return m_height; }
        int channels() const { return m_channels; }

      private:
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
    };
}  // namespace gl