#pragma once

#include <vector>
#include "../GLTypes.h"


namespace gl {

    class TextureRef;
    class Texture2D;

    class BindlessTextures {
      public:
        BindlessTextures();
        ~BindlessTextures();

        size_t add(Texture2D&& texture);
        size_t add(const TextureRef& texture);

        void use();
        void use(size_t index);

        void unload();
        void unload(size_t index);

        void destroy(size_t index);
        void update();
        void bind(unsigned bindingPoint);
        void unbind(unsigned bindingPoint);

        void setUniform(const char* name, size_t index, unsigned int programID);

      private:
        std::vector<TextureRef> m_textures;
        std::vector<GLuint64> m_textureHandles;
        bool m_dirty = false;
        bool m_resident = false;

        unsigned int m_ssboID = 0;
    };
}  // namespace gl
