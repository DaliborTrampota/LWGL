#include "LWGL/buffer/FBO.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/buffer/RBO.h"


#include <glad/glad.h>
#include <algorithm>
#include <stdexcept>

#include "LWGL/texture/CubeMap.h"
#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/Texture2D.h"
#include "LWGL/texture/TextureArray.h"


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

FBO::FBO() : m_target(GL_FRAMEBUFFER), m_attachments(), m_textures() {
    glGenFramebuffers(1, &m_fboID);
    if (MaxDrawBuffers == -1) {
        MaxDrawBuffers = queryMaxDrawBuffers();
    }
    if (MaxColorAttachments == -1) {
        MaxColorAttachments = queryMaxColorAttachments();
    }
}

void FBO::bindTexture(Att attachment, const TextureBase* texture) {
    if (attachment - FBOAttachment::Color >= MaxColorAttachments)
        throw std::runtime_error("Exceeding max color attachments");

    auto attIt = findAtt(m_attachments, attachment);
    if (attIt == m_attachments.end()) {
        m_attachments.push_back(attachment);
        m_textures.push_back(texture);
    } else {
        int index = std::distance(m_attachments.cbegin(), attIt);
        if (m_textures[index] != nullptr) {
            unsigned int prevTexID = m_textures[index]->id();
            glDeleteTextures(1, &prevTexID);
        }
        m_textures[index] = texture;
    }

    unsigned int texID = texture->id();
    if (auto tex = dynamic_cast<const Texture2D*>(texture)) {
        glBindTexture(GL_TEXTURE_2D, tex->id());
        glFramebufferTexture2D(
            m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, texID, 0
        );
    } else if (auto tex = dynamic_cast<const TextureArray*>(texture)) {
        glBindTexture(GL_TEXTURE_2D_ARRAY, tex->id());
        glFramebufferTexture(m_target, detail::toGLAttachmentType(attachment), texID, 0);
    } else if (auto tex = dynamic_cast<const CubeMap*>(texture)) {
        // glBindTexture(GL_TEXTURE_CUBE_MAP, tex->id());
        throw std::runtime_error("Unsupported texture type");
    } else {
        throw std::runtime_error("Unsupported texture type");
    }
}

void FBO::createTexture(Att attachment, const FrameBufferSettings& settings) {
    if (attachment - FBOAttachment::Color >= MaxColorAttachments)
        throw std::runtime_error("Exceeding max color attachments");
    if (findAtt(m_attachments, attachment) != m_attachments.end())
        throw std::runtime_error("Attachment already exists");

    Texture2D* texture = new Texture2D();
    texture->create(settings);
    texture->bind();
    texture->load(
        ImageData(nullptr, settings.width, settings.height, 4, settings.format, settings.dataType)
    );

    m_textures.push_back(texture);
    m_attachments.push_back(attachment);

    // bind();
    // glFramebufferTexture2D(
    //     m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, texture->id(), 0
    // );
    glNamedFramebufferTexture(m_fboID, detail::toGLAttachmentType(attachment), texture->id(), 0);
    //unbind();
}

void FBO::removeAttachment(Att attachment) {
    auto it = findAtt(m_attachments, attachment);
    if (it == m_attachments.end())
        throw std::runtime_error("Attachment not found");
    int index = std::distance(m_attachments.cbegin(), it);

    glFramebufferTexture2D(m_target, detail::toGLAttachmentType(attachment), GL_TEXTURE_2D, 0, 0);
    m_textures.erase(m_textures.begin() + index);
    m_attachments.erase(it);
    delete m_textures[index];
    glBindFramebuffer(m_target, 0);  // TODO remove
}

void FBO::clearActive(const glm::vec4& color, float depth, uint8_t stencil) const {
    glBindFramebuffer(m_target, m_fboID);
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(depth);
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FBO::clearColor(uint8_t index, const glm::vec4& color) const {
    glBindFramebuffer(m_target, m_fboID);
    glClearBufferfv(GL_COLOR, index, &color.r);
}

void FBO::clearDepth(float depth) const {
    glBindFramebuffer(m_target, m_fboID);
    glClearBufferfv(GL_DEPTH, 0, &depth);
}

void FBO::clearDepthStencil(float depth, uint8_t stencil) const {
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
    // switch (status) {
    //     case GL_FRAMEBUFFER_COMPLETE: return 0;
    //     case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return 1;
    //     case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return 2;
    //     case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return 3;
    //     case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return 4;
    //     case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return 5;
    //     case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return 6;
    //     case GL_FRAMEBUFFER_UNSUPPORTED: return 7;
    //     case GL_FRAMEBUFFER_UNDEFINED: return 8;
    // }
    return status == GL_FRAMEBUFFER_COMPLETE ? 0 : status;
}


void FBO::attachRenderBuffer(RBO& rbo, Att attachment, Target target) {
    bind();
    glFramebufferRenderbuffer(
        toGLFBOTarget(target), detail::toGLAttachmentType(attachment), GL_RENDERBUFFER, rbo.id()
    );
}

const TextureBase* FBO::texture(Att attachment) const {  // TODO better lookup and storage
    auto it = findAtt(m_attachments, attachment);
    if (it == m_attachments.cend())
        throw std::runtime_error("Attachment not found");
    return m_textures[std::distance(m_attachments.cbegin(), it)];
}