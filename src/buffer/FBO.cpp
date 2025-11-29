#include "LWGL/buffer/FBO.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/buffer/RBO.h"


#include <glad/glad.h>
#include <algorithm>
#include <stdexcept>


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

    inline std::vector<FBO::Att>::const_iterator findAtt(
        const std::vector<FBO::Att>& attachments, FBO::Att attachment
    ) {
        return std::find(attachments.cbegin(), attachments.cend(), attachment);
    }
    int queryMaxDrawBuffers() {
        int maxDrawBuffers;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxDrawBuffers);
        return maxDrawBuffers;
    }

    int queryMaxColorAttachments() {
        int maxColorAttachments;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        return maxColorAttachments;
    }

}  // namespace

FBO::FBO() : m_target(GL_FRAMEBUFFER), m_attachments(), m_texIDs() {
    glGenFramebuffers(1, &m_fboID);
    if (MaxDrawBuffers == -1) {
        MaxDrawBuffers = queryMaxDrawBuffers();
    }
    if (MaxColorAttachments == -1) {
        MaxColorAttachments = queryMaxColorAttachments();
    }
}

void FBO::bindTexture(Att attachment, unsigned int textureID) {
    if (attachment - FBOAttachment::Color >= MaxColorAttachments)
        throw std::runtime_error("Exceeding max color attachments");

    auto attIt = findAtt(m_attachments, attachment);
    if (attIt == m_attachments.end()) {
        m_attachments.push_back(attachment);
        m_texIDs.push_back(textureID);
    } else {
        int index = std::distance(m_attachments.cbegin(), attIt);
        if (m_texIDs[index] != 0) {
            glDeleteTextures(1, &m_texIDs[index]);
        }
        m_texIDs[index] = textureID;
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glFramebufferTexture2D(
        m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, textureID, 0
    );
}

void FBO::createTexture(Att attachment, const FrameBufferSettings& settings) {
    if (attachment - FBOAttachment::Color >= MaxColorAttachments)
        throw std::runtime_error("Exceeding max color attachments");
    if (findAtt(m_attachments, attachment) != m_attachments.end())
        throw std::runtime_error("Attachment already exists");

    unsigned int texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    detail::Data2D(
        GL_TEXTURE_2D, settings.width, settings.height, settings.format, nullptr, settings.dataType
    );
    detail::ConfigureTexture(GL_TEXTURE_2D, settings);

    m_texIDs.push_back(texID);
    m_attachments.push_back(attachment);
    bind();
    glFramebufferTexture2D(
        m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, texID, 0
    );
    //glNamedFramebufferTexture(m_fboID, detail::toGLAttachmentType(attachment), texID, 0);
    //unbind();
}

void FBO::removeAttachment(Att attachment) {
    auto it = findAtt(m_attachments, attachment);
    if (it == m_attachments.end())
        throw std::runtime_error("Attachment not found");
    int index = std::distance(m_attachments.cbegin(), it);

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


void FBO::bind() const {
    glBindFramebuffer(m_target, m_fboID);
}

void FBO::unbind() const {
    glBindFramebuffer(m_target, 0);
}

void FBO::setDrawBuffers(std::initializer_list<Att> colorAttachments) const {
    if (m_target != GL_DRAW_FRAMEBUFFER && m_target != GL_FRAMEBUFFER) {
        throw std::runtime_error("Cannot set draw buffers on a non-draw framebuffer");
    }

    std::vector<GLenum> glAttachments;
    glAttachments.reserve(colorAttachments.size());
    for (Att attachment : colorAttachments)
        glAttachments.push_back(detail::toGLAttachmentType(attachment));

    glNamedFramebufferDrawBuffers(m_fboID, glAttachments.size(), glAttachments.data());
}

void FBO::setReadBuffer(Att colorAttachment) const {
    if (m_target != GL_READ_FRAMEBUFFER && m_target != GL_FRAMEBUFFER) {
        throw std::runtime_error("Cannot set read buffer on a non-read framebuffer");
    }
    glNamedFramebufferReadBuffer(m_fboID, detail::toGLAttachmentType(colorAttachment));
}


unsigned int FBO::checkCompleteness() const {
    unsigned int status = glCheckNamedFramebufferStatus(m_fboID, m_target);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE: return 0;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return 1;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return 2;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return 3;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return 4;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return 5;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return 6;
        case GL_FRAMEBUFFER_UNSUPPORTED: return 7;
        case GL_FRAMEBUFFER_UNDEFINED: return 8;
    }
    return status == GL_FRAMEBUFFER_COMPLETE ? 0 : status;
}


void FBO::attachRenderBuffer(RBO& rbo, Att attachment, Target target) {
    bind();
    glFramebufferRenderbuffer(
        toGLFBOTarget(target), detail::toGLAttachmentType(attachment), GL_RENDERBUFFER, rbo.id()
    );
}

unsigned FBO::texture(Att attachment) const {  // TODO better lookup and storage
    return m_texIDs[std::distance(m_attachments.cbegin(), findAtt(m_attachments, attachment))];
}