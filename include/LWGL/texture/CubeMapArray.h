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

        int width() { return m_width; }
        int channels() { return m_channels; }

        int layers() { return m_layers; }
        int layerFaces() { return m_layers * 6; }

      private:
        int m_width = 0;
        int m_layers = 0;
        int m_channels = 0;
    };
}  // namespace gl