#include "LWGL/buffer/RBO.h"
#include "../detail/TexturePrivate.h"

#include <glad/glad.h>


using namespace gl;

RBO::RBO() {}

RBO::~RBO() {
    if (m_ID != 0) {
        glDeleteRenderbuffers(1, &m_ID);
    }
}

void RBO::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RBO::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::create(unsigned int width, unsigned int height, GLenum internalFormat, int samples) {
    if (!m_ID)
        glGenRenderbuffers(1, &m_ID);

    // TODO If internalformat​ is a signed or unsigned integer format then samples​ must be less than or equal to the value of GL_MAX_INTEGER_SAMPLES.
    if (samples > GL_MAX_SAMPLES)
        samples = GL_MAX_SAMPLES;

    m_format = internalFormat;
    m_samples = samples;
    m_width = width;
    m_height = height;

    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
}