#include "LWGL/texture/TextureBase.h"
#include "../detail/conversions.h"

#include <glad/glad.h>
#include <stdexcept>

using namespace gl;

TextureBase::TextureBase(TextureType type) : m_type(type) {}

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