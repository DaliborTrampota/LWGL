#pragma once

#include "LWGL/texture/ImageData.h"

namespace gl {
    class FBO;

    class RBO {
      public:
        RBO();
        ~RBO();

        void bind();
        void unbind();

        void create(unsigned int width, unsigned int height, ImageFormat format, int samples = 0);

        unsigned int id() const { return m_ID; }
        ImageFormat format() const { return m_format; }
        int samples() const { return m_samples; }
        unsigned int width() const { return m_width; }
        unsigned int height() const { return m_height; }


      protected:
        ImageFormat m_format = ImageFormat::Unknown;
        unsigned int m_ID = 0;
        int m_samples = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
    };
}  // namespace gl