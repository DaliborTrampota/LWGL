#include "LWGL/texture/TextureRef.h"
#include "../detail/conversions.h"


using namespace gl;

TextureRef::TextureRef(const TextureBase* texture) : TextureBase(detail::textureType(texture)) {
    m_id = texture->id();
}

TextureRef::TextureRef(unsigned int textureID, TextureType type) : TextureBase(type) {
    m_id = textureID;
}