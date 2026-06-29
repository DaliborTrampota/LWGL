#pragma once

#include "TextureBase.h"

namespace gl {

    class CubeMapArray : public TextureBase {
      public:
        CubeMapArray(bool immutable = true) : TextureBase(TextureType::CubeMapArray, immutable) {}

        void create(TextureParams params);
        void allocate(TextureStorage storage);
        void upload(int layer, CubeFace face, int width, ImageFormat format, Data data);
        // void upload(int layer, CubeFace face, const ImageData& imageData);
        // void upload(int layer, CubeFace face, const RawImageData& rawImageData);

        int width() const { return m_width; }
        int channels() const { return m_channels; }

        uint32_t layers() const { return m_layers; }
        uint32_t layerFaces() const { return m_layers * 6; }

      private:
        int m_width = 0;
        uint32_t m_layers = 0;
        int m_channels = 0;
    };
}  // namespace gl