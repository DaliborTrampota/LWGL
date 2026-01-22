#pragma once

#include "TextureBase.h"

namespace gl {


    class TextureRef : public TextureBase {
      public:
        TextureRef(const TextureBase* texture);
        TextureRef(unsigned int textureID, TextureType type);
        ~TextureRef() {
            // Just set to 0 to avoid TextureBase dtor glDeleting the texture since it's just a reference
            m_id = 0;
        };

        TextureType type() const { return m_type; }
    };
}  // namespace gl