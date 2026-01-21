#pragma once

#include "TextureBase.h"

namespace gl {
    class TextureRef : public TextureBase {
      public:
        TextureRef(unsigned int textureID, TextureType type) : TextureBase(type) {
            m_id = textureID;
        }
        ~TextureRef() {
            // Just set to 0 to avoid TextureBase dtor glDeleteing the texture since its just a reference
            m_id = 0;
        };

        TextureType type() const { return m_type; }
    };
}  // namespace gl