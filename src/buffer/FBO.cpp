#include "LWGL/buffer/FBO.h"
#include "../detail/TexturePrivate.h"
#include "LWGL/buffer/RBO.h"


#include <glad/glad.h>


using namespace gl;

namespace {
    constexpr GLenum toGLFBOTarget(FBO::Target target) {
        switch (target) {
            case FBO::Target::Draw: return GL_DRAW_FRAMEBUFFER;
            case FBO::Target::Read: return GL_READ_FRAMEBUFFER;
            case FBO::Target::ReadDraw: return GL_FRAMEBUFFER;
        }
        return GL_NONE;  // Should not happen
    }
}  // namespace

FBO::FBO(std::initializer_list<FBOAttachment> attachments, Target target)
    : m_target(toGLFBOTarget(target)),
      m_attachments(attachments),
      m_texIDs(attachments.size(), 0) {
    glGenFramebuffers(1, &m_fboID);
}

void FBO::bindTexture(FBOAttachment attachment, unsigned int textureID) {
    auto it = std::find(m_attachments.begin(), m_attachments.end(), attachment);
    if (it == m_attachments.end()) {
        m_attachments.push_back(attachment);
        m_texIDs.push_back(textureID);
    } else {
        int index = std::distance(m_attachments.begin(), it);
        if (m_texIDs[index] != 0) {
            glDeleteTextures(1, &m_texIDs[index]);
        }
        m_texIDs[index] = textureID;
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, textureID, 0
    );
}

void FBO::createTexture(FBOAttachment attachment, const FrameBufferSettings& settings) {
    if (std::find(m_attachments.begin(), m_attachments.end(), attachment) != m_attachments.end()) {
        throw std::runtime_error("Attachment already exists");
    }
    unsigned int texID;
    glGenTextures(1, &texID);

    detail::Data2D(
        GL_TEXTURE_2D, settings.width, settings.height, settings.format, nullptr, settings.dataType
    );
    detail::ConfigureTexture(GL_TEXTURE_2D, settings);

    glBindTexture(GL_TEXTURE_2D, texID);
    m_texIDs.push_back(texID);
    m_attachments.push_back(attachment);
    glFramebufferTexture2D(
        m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, texID, 0
    );
}

void FBO::removeAttachment(FBOAttachment attachment) {
    auto it = std::find(m_attachments.begin(), m_attachments.end(), attachment);
    if (it == m_attachments.end()) {
        throw std::runtime_error("Attachment not found");
    }
    int index = std::distance(m_attachments.begin(), it);
    glFramebufferTexture2D(m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, 0, 0);
    glDeleteTextures(1, &m_texIDs[index]);
    m_texIDs.erase(m_texIDs.begin() + index);
    m_attachments.erase(it);
    glBindFramebuffer(m_target, 0);
}

void FBO::clearActive(const glm::vec4& color, float depth, uint8_t stencil) {
    glBindFramebuffer(m_target, m_fboID);
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(depth);
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FBO::clearColor(uint8_t index, const glm::vec4& color) {
    glBindFramebuffer(m_target, m_fboID);
    glClearBufferfv(GL_COLOR, index, &color.r);
}

void FBO::clearDepth(float depth) {
    glBindFramebuffer(m_target, m_fboID);
    glClearBufferfv(GL_DEPTH, 0, &depth);
}

void FBO::clearDepthStencil(float depth, uint8_t stencil) {
    glBindFramebuffer(m_target, m_fboID);
    glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}


void FBO::bind() {
    glBindFramebuffer(m_target, m_fboID);
}

void FBO::unbind() {
    glBindFramebuffer(m_target, 0);
}

void FBO::setDrawBuffers(std::initializer_list<uint8_t> colorAttachmentIndices) {
    if (m_target != GL_DRAW_FRAMEBUFFER && m_target != GL_FRAMEBUFFER) {
        throw std::runtime_error("Cannot set draw buffers on a non-draw framebuffer");
    }
    std::vector<GLenum> glAttachments;
    glAttachments.reserve(colorAttachmentIndices.size());

    for (uint8_t index : colorAttachmentIndices) {
        glAttachments.push_back(GL_COLOR_ATTACHMENT0 + index);
    }

    glNamedFramebufferDrawBuffers(m_fboID, glAttachments.size(), glAttachments.data());
}

void FBO::setReadBuffer(uint8_t colorAttachmentIndex) {
    if (m_target != GL_READ_FRAMEBUFFER && m_target != GL_FRAMEBUFFER) {
        throw std::runtime_error("Cannot set read buffer on a non-read framebuffer");
    }
    if (colorAttachmentIndex >= m_attachments.size()) {
        throw std::runtime_error("Color attachment index out of bounds");
    }
    glNamedFramebufferReadBuffer(m_fboID, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
}


unsigned int FBO::checkCompleteness() const {
    unsigned int status = glCheckNamedFramebufferStatus(m_fboID, m_target);
    return status == GL_FRAMEBUFFER_COMPLETE ? 0 : status;
}


void FBO::attachRenderBuffer(RBO& rbo, FBOAttachment attachment, Target target) {
    bind();
    glFramebufferRenderbuffer(
        toGLFBOTarget(target), detail::toGLAttachmentType(attachment), GL_RENDERBUFFER, rbo.id()
    );
}