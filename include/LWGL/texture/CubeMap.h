#pragma once

#include "TextureBase.h"

namespace gl {

    class CubeMap : public TextureBase {
      public:
        CubeMap(bool immutable = true) : TextureBase(TextureType::CubeMap, immutable) {};

        /// @brief Creates a cube map from 6 ImageData
        /// @note The textures are in the order of CubeFace enum: Right, Left, Top, Bottom, Front, Back
        static CubeMap fromImageData(
            ImageData imageData[6], TextureParams params, bool immutable = true
        );
        // static Texture2D forRenderTarget(TextureStorage storage, TextureParams params);

        /// @brief Creates and configures the texture
        void create(TextureParams params);

        /// @brief Allocates memory for the texture
        /// @note This is not needed for mutable cube maps, it does nothing
        void allocate(TextureStorage storage);

        /// @brief Uploads raw data to the texture
        void upload(CubeFace face, int width, ImageFormat format, Data data);

        /// @brief Uploads data to the texture from ImageData
        /// @note There is implicit conversion from resource path to ImageData
        void upload(CubeFace face, const ImageData& imageData);

        /// @brief Uploads data to the texture from RawImageData
        void upload(CubeFace face, const RawImageData& rawImageData);

        int width() { return m_width; }

      private:
        int m_width = 0;
        int m_channels = 0;
    };
}  // namespace gl