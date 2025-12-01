#pragma once

#include "LWGL/texture/TextureBase.h"

#include <glm/glm.hpp>
#include <vector>


namespace gl {

    class RBO;
    class FBO {
      public:
        using Att = FBOAttachment::Attachment;
        enum Target : uint8_t {
            Draw,
            Read,
            ReadDraw,
        };

        static inline int MaxDrawBuffers = -1;
        static inline int MaxColorAttachments = -1;

        FBO();
        // FBO(std::initializer_list<FBOAttachment> attachments, Target target = Target::ReadDraw);

        void bind() const;
        void unbind() const;

        /// @brief Sets where draw operations will write to.
        /// @param colorAttachments The color attachments to write to.
        void setDrawBuffers(std::initializer_list<Att> colorAttachments) const;
        /// @brief Sets where read operations will read from.
        /// @param colorAttachment The color attachment to read from.
        void setReadBuffer(Att colorAttachment) const;

        /// @brief Binds a texture to an attachment. If the attachment is already bound, the texture is replaced, old texture is deleted.
        /// @param attachment Attachment to bind the texture to.
        /// @param textureID ID of the texture to bind.
        void bindTexture(Att attachment, unsigned int textureID);

        /// @brief Creates a texture for an attachment.
        /// @param attachment Attachment to create the texture for.
        /// @param settings Settings for the texture.
        void createTexture(Att attachment, const FrameBufferSettings& settings);

        /// @brief Removes a texture and attachment from this FBO.
        /// @param attachment Attachment to remove.
        /// @note The texture is deleted and the attachment is removed from the FBO.
        void removeAttachment(Att attachment);

        /// @brief Clears the active buffer. (Buffers bound to glDrawBuffers)
        /// @param color Color to clear the attachment to.
        /// @param depth Depth to clear the attachment to.
        /// @param stencil Stencil to clear the attachment to.
        /// @note Active means the attachment is bound to glDrawBuffer.
        void clearActive(
            const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            float depth = 1.0f,
            uint8_t stencil = 0
        ) const;

        /// @brief Clears a color attachment.
        /// @param colorAttachment The color attachment to clear.
        /// @param color Color to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the color is not cleared.
        void clearColor(
            Att colorAttachment, const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
        ) const;

        /// @brief Clears the depth attachment.
        /// @param depth Depth to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the depth is not cleared.
        void clearDepth(float depth = 1.0f) const;

        /// @brief Clears the depth and stencil attachment.
        /// @param depth Depth to clear the attachment to.
        /// @param stencil Stencil to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the depth and stencil are not cleared.
        void clearDepthStencil(float depth = 1.0f, uint8_t stencil = 0) const;

        /// @brief Checks if the FBO is complete based on OpenGL specification
        /// @see https://wikis.khronos.org/opengl/Framebuffer_Object#Framebuffer_Completeness
        /// @return 0 if the FBO is complete, otherwise the OpenGL error code.
        /// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCheckFramebufferStatus.xhtml
        unsigned int checkCompleteness() const;

        void attachRenderBuffer(RBO& rbo, Att attachment, Target target);

        unsigned texture(Att attachment) const;

      protected:
        unsigned int m_fboID = 0;
        unsigned int m_target;
        std::vector<unsigned> m_texIDs;
        std::vector<Att> m_attachments;
    };
}  // namespace gl