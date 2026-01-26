#pragma once

#include "TextureBase.h"

namespace gl {
    class Texture1D : public TextureBase {
      public:
        Texture1D(bool immutable = true) : TextureBase(TextureType::Texture1D, immutable) {};

        /// @brief Creates a 1D texture from ImageData
        static Texture1D fromImageData(
            const ImageData& imageData, TextureParams params, bool immutable = true
        );
        /// @brief Creates a 1D texture from RawImageData
        static Texture1D fromRawData(
            const RawImageData& rawImageData, TextureParams params, bool immutable = true
        );
        /// @brief Creates a 1D texture for render target (create and allocate)
        static Texture1D forRenderTarget(
            TextureStorage storage, TextureParams params, bool immutable = true
        );

        /// @brief Creates and configures the texture
        void create(TextureParams params);

        /// @brief Allocates memory for the texture
        void allocate(TextureStorage storage);

        /// @brief Uploads raw data to the texture
        void upload(ImageFormat format, Data data);

        int width() const { return m_width; }
        int channels() const { return m_channels; }

      private:
        int m_width = 0;
        int m_channels = 0;
    };
}  // namespace gl