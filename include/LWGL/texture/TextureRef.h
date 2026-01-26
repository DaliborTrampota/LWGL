#pragma once

#include "TextureBase.h"

namespace gl {


    class TextureRef : public TextureBase {
      public:
        TextureRef(const TextureBase* texture) : TextureBase(texture->type(), false) {
            m_id = texture->id();
        }
        TextureRef(unsigned int textureID, TextureType type, bool owned = false)
            : TextureBase(type, false),
              m_owned(owned) {
            m_id = textureID;
        }
        ~TextureRef() {
            if (!m_owned) {
                // Just set to 0 to avoid TextureBase dtor glDeleting the texture since it's just a reference
                m_id = 0;
            }
        };

        TextureType type() const { return m_type; }
        // TODO hold weak ptr and check if texture is still valid

      private:
        bool m_owned = false;
    };
}  // namespace gl