#include "LWGL/texture/TextureRef.h"

using namespace gl;


TextureRef::TextureRef(const TextureRef& other) : TextureBase(other.type(), other.m_immutable) {
    m_id = other.m_id;
    m_owned = false;
}

TextureRef& TextureRef::operator=(const TextureRef& other) {
    if (this != &other) {
        m_id = other.m_id;
        m_owned = false;
        m_type = other.m_type;
        m_immutable = other.m_immutable;
    }
    return *this;
}

TextureRef::TextureRef(TextureRef&& other) noexcept : TextureBase(other.type(), other.m_immutable) {
    m_id = std::exchange(other.m_id, 0);
    m_owned = other.m_owned;
}

TextureRef& TextureRef::operator=(TextureRef&& other) noexcept {
    if (this != &other) {
        m_id = std::exchange(other.m_id, 0);
        m_owned = other.m_owned;
        m_type = other.m_type;
        m_immutable = other.m_immutable;
    }
    return *this;
}


TextureRef::TextureRef(const TextureBase* texture) : TextureBase(texture->type(), false) {
    m_id = texture->id();
}

TextureRef::TextureRef(unsigned int textureID, TextureType type, bool owned)
    : TextureBase(type, false),
      m_owned(owned) {
    m_id = textureID;
}
TextureRef::~TextureRef() {
    if (!m_owned) {
        // Just set to 0 to avoid TextureBase dtor glDeleting the texture since it's just a reference
        m_id = 0;
    }
};