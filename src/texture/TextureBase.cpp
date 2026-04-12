#include "LWGL/texture/TextureBase.h"
#include "../detail/conversions.h"

#include <glad/glad.h>
#include <stdexcept>
#include <utility>


using namespace gl;


TextureBase::TextureBase(TextureBase&& other) noexcept
    : m_id(other.m_id),
      m_type(other.m_type),
      m_immutable(other.m_immutable) {
    other.m_id = 0;
}


TextureBase::TextureBase(TextureType type, bool immutable) : m_type(type), m_immutable(immutable) {}

TextureBase::~TextureBase() {
    glDeleteTextures(1, &m_id);
}

void TextureBase::bind() const {
    glBindTexture(detail::toGLTexture(m_type), m_id);
}

void TextureBase::activate(UInt unit) const {
    if (unit >= 32)
        throw std::runtime_error("Texture unit out of range");

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(detail::toGLTexture(m_type), m_id);
}
void TextureBase::unbind(UInt unit) const {
    if (unit >= 32)
        throw std::runtime_error("Texture unit out of range");

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(detail::toGLTexture(m_type), 0);
}

void TextureBase::unbind(TextureType type) {
    glBindTexture(detail::toGLTexture(type), 0);
}

UInt TextureBase::release() {
    return std::exchange(m_id, 0);
}
