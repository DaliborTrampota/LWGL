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
        void use(int index);

        void unload();
        void unload(int index);

        void destroy(int index);
        void update();
        void bind(unsigned bindingPoint);

        void setUniform(const char* name, int index, unsigned int programID);

      private:
        std::vector<TextureRef> m_textures;
        std::vector<GLuint64> m_textureHandles;
        bool m_dirty = false;

        unsigned int m_ssboID = 0;
    };
}  // namespace gl
