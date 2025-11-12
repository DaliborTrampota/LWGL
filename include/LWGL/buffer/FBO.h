#pragma once

#include "LWGL/texture/TextureBase.h"

#include <vector>

#include <glm/glm.hpp>

namespace gl {
    class RBO;

    class FBO {
      public:
        enum Target : uint8_t {
            Draw,
            Read,
            ReadDraw,
        };

        FBO(std::initializer_list<FBOAttachment> attachments, Target target = Target::ReadDraw);

        void bind();
        void unbind();

        /// @brief Sets where draw operations will write to.
        /// @param colorAttachmentIndices Indices of the color attachments to write to.
        void setDrawBuffers(std::initializer_list<uint8_t> colorAttachmentIndices);
        /// @brief Sets where read operations will read from.
        /// @param colorAttachmentIndex Index of the color attachment to read from.
        void setReadBuffer(uint8_t colorAttachmentIndex);

        /// @brief Binds a texture to an attachment. If the attachment is already bound, the texture is replaced, old texture is deleted.
        /// @param attachment Attachment to bind the texture to.
        /// @param textureID ID of the texture to bind.
        void bindTexture(FBOAttachment attachment, unsigned int textureID);

        /// @brief Creates a texture for an attachment.
        /// @param attachment Attachment to create the texture for.
        /// @param settings Settings for the texture.
        void createTexture(FBOAttachment attachment, const FrameBufferSettings& settings);

        /// @brief Removes a texture and attachment from this FBO.
        /// @param attachment Attachment to remove.
        /// @note The texture is deleted and the attachment is removed from the FBO.
        void removeAttachment(FBOAttachment attachment);

        /// @brief Clears the active buffer. (Buffers bound to glDrawBuffers)
        /// @param color Color to clear the attachment to.
        /// @param depth Depth to clear the attachment to.
        /// @param stencil Stencil to clear the attachment to.
        /// @note Active means the attachment is bound to glDrawBuffer.
        void clearActive(
            const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            float depth = 1.0f,
            uint8_t stencil = 0
        );

        /// @brief Clears a color attachment.
        /// @param index Index of the attachment to clear.
        /// @param color Color to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the color is not cleared.
        void clearColor(
            uint8_t index = 0, const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
        );

        /// @brief Clears the depth attachment.
        /// @param depth Depth to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the depth is not cleared.
        void clearDepth(float depth = 1.0f);

        /// @brief Clears the depth and stencil attachment.
        /// @param depth Depth to clear the attachment to.
        /// @param stencil Stencil to clear the attachment to.
        /// @note If the attachment is not glDrawBuffer bound, the depth and stencil are not cleared.
        void clearDepthStencil(float depth = 1.0f, uint8_t stencil = 0);

        /// @brief Checks if the FBO is complete based on OpenGL specification
        /// @see https://wikis.khronos.org/opengl/Framebuffer_Object#Framebuffer_Completeness
        /// @return 0 if the FBO is complete, otherwise the OpenGL error code.
        /// @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCheckFramebufferStatus.xhtml
        unsigned int checkCompleteness() const;

        void attachRenderBuffer(RBO& rbo, FBOAttachment attachment, Target target);


      protected:
        unsigned int m_fboID = 0;
        unsigned int m_target;
        std::vector<unsigned int> m_texIDs;
        std::vector<FBOAttachment> m_attachments;
    };
}  // namespace gl