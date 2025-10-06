#pragma once

#include "LWGL/texture/TextureBase.h"

namespace gl {

    class FBO {
        public:
            FBO(FBOType type);

            void bind();
            void bindTexture(unsigned int textureID);
            void createTexture(const FrameBufferSettings& settings);
            
            void clear();
            void clearColor();
            void clearDepth();
            void clearStencil();
            

        protected:
            unsigned int m_fboID = 0;
            unsigned int m_texID = 0;
            FBOType m_type;

    };
}