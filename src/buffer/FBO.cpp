#include "LWGL/buffer/FBO.h"

#include <glad/glad.h>

#include "../detail/TexturePrivate.h"
#include "LWGL/buffer/FBO.h"

using namespace gl;

FBO::FBO(FBOType type) : m_type(type) {}

void FBO::bindTexture(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    m_texID = textureID;
}

void FBO::createTexture(const FrameBufferSettings& settings) {
    glGenTextures(1, &m_texID);

    detail::Data2D(GL_TEXTURE_2D, settings.width, settings.height, settings.format, nullptr, settings.dataType);
    detail::ConfigureTexture(GL_TEXTURE_2D, settings);

    glBindTexture(GL_TEXTURE_2D, m_texID);
}   

void FBO::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FBO::clearColor() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void FBO::clearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void FBO::clearStencil() {
    glClear(GL_STENCIL_BUFFER_BIT);
}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, detail::toGLAttachmentType(m_type), GL_TEXTURE_2D, m_texID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}


