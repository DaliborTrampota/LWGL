#include "LWGL/texture/TextureBase.h"
#include "../detail/TexturePrivate.h"

#include <glad/glad.h>
#include <stdexcept>

using namespace gl;

TextureBase::TextureBase(UInt unit, Type type) : m_unit(unit), m_type(type) {
    if (unit >= 32)
        throw std::runtime_error("Texture unit out of range");
}

TextureBase::~TextureBase() {
    glDeleteTextures(1, &m_id);
}

void TextureBase::bind() const {
    glBindTexture(detail::toGLType(m_type), m_id);
}
void TextureBase::unbind() const {
    glBindTexture(detail::toGLType(m_type), 0);
}