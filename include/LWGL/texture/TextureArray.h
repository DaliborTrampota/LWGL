#pragma once

#include "TextureBase.h"

namespace gl {

    class TextureArray : public TextureBase {
      public:
        TextureArray(UInt unit) : TextureBase(unit, TextureType::TextureArray) {}

        void create(ArraySettings settings);
        void create(RawArraySettings settings);
        int load(const ImageData& imageData, int layer = -1);
        void load(const RawImageData& rawImageData, int layer = -1);

        /// @brief Load raw data into the texture
        /// @param data The data to load, has to match the width and height of the texture
        /// @param format The format of the data
        /// @param layer The layer to load the data into, if -1, the next available layer will be used
        /// @return The layer that the data was loaded into
        int loadRaw(Data data, ImageFormat format, int layer = -1);

      private:
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        GLenum m_format = GL_NONE;
        int m_layer = 0;
        unsigned int m_maxLayers = 0;
    };
}  // namespace gl