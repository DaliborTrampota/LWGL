#pragma once

#include <cstddef>
#include <vector>

#include <glm/glm.hpp>
#include "../buffer/SSBO.h"
#include "IndirectTypes.h"

namespace gl {

    class IndirectBuffer {
      public:
        IndirectBuffer(size_t capacity = 2000);
        ~IndirectBuffer();

        void begin();
        void add(PoolAllocation alloc, const glm::mat4& model);
        void upload();
        void draw() const;

        uint32_t drawCount() const { return m_commands.size(); }
        uint32_t verticesCount() const { return m_vertCount; }

      private:
        GLuint m_IB = 0;
        uint32_t m_vertCount = 0;
        size_t m_gpuCapacity = 0;

        std::vector<DrawArraysIndirectCommand> m_commands;

        GLuint m_modelBindPoint = 0;
        SSBO<glm::mat4> m_models{GL_STREAM_DRAW};
    };
}  // namespace gl