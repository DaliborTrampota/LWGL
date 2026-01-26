#pragma once

#include "TextureBase.h"

namespace gl {

    class TextureArray : public TextureBase {
      public:
        TextureArray(bool immutable = true) : TextureBase(TextureType::TextureArray, immutable) {}

        // static TextureArray fromImageData(const ImageData& imageData, TextureParams params);
        // static TextureArray fromRawData(const RawImageData& rawImageData, TextureParams params);
        // static TextureArray forRenderTarget(TextureStorage storage, TextureParams params);

        /// @brief Creates and configures the texture
        void create(TextureParams params);

        /// @brief Allocates memory for the texture
        void allocate(TextureStorage storage);

        /// @brief Load raw data into the texture
        /// @param format The format of the data
        /// @param data The data to load, has to match the width and height of the texture
        /// @param layer The layer to load the data into, if -1, the next available layer will be used
        /// @return The layer that the data was loaded into
        /// @note The data has to match the width and height of the texture
        /// @note OpenGL will throw an error if the ImageFormat is not compatible with the allocated ImageFormat from TextureArray::allocate
        int upload(ImageFormat format, Data data, int layer = -1);

        /// @brief Load image data into the texture
        /// @param imageData The image data to load
        /// @param layer The layer to load the data into, if -1, the next available layer will be used
        /// @return The layer that the data was loaded into
        /// @note The image data has to match (or be smaller than) the width and height of the texture
        /// @note OpenGL will throw an error if the ImageFormat is not compatible with the allocated ImageFormat from TextureArray::allocate
        int upload(const ImageData& imageData, int layer = -1);

        /// @brief Load raw data into the texture
        /// @param rawImageData The raw data to load
        /// @param layer The layer to load the data into, if -1, the next available layer will be used
        /// @return The layer that the data was loaded into
        /// @note The raw data has to match (or be smaller than) the width and height of the texture
        /// @note OpenGL will throw an error if the ImageFormat is not compatible with the allocated ImageFormat from TextureArray::allocate
        int upload(const RawImageData& rawImageData, int layer = -1);

      private:
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        GLenum m_format = GL_NONE;
        int m_layer = 0;
        unsigned int m_maxLayers = 0;

        int compatibilityCheck(int w, int h, int layer);
    };
}  // namespace gl