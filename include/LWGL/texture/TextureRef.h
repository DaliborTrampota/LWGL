#pragma once

#include "TextureBase.h"

namespace gl {


    class TextureRef : public TextureBase {
      public:
        TextureRef(const TextureBase* texture);
        TextureRef(unsigned int textureID, TextureType type, bool owned = false);
        ~TextureRef();

        /// @note If the texture that is being copied is owning, the copy will not be owning. Only one copy of the texture can be owned.
        TextureRef(const TextureRef&);
        /// @note If the texture that is being copied is owning, the copy will not be owning. Only one copy of the texture can be owned.
        TextureRef& operator=(const TextureRef&);
        TextureRef(TextureRef&& other) noexcept;
        TextureRef& operator=(TextureRef&& other) noexcept;

        TextureType type() const { return m_type; }
        // TODO hold weak ptr and check if texture is still valid

      private:
        bool m_owned = false;
    };
}  // namespace gl