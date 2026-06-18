#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <vector>


#include "../buffer/VertexLayout.h"
#include "IndirectTypes.h"


namespace gl {

    template <gl::VertexType T>
    class VertexPool {
      public:
        explicit VertexPool(uint32_t size);
        ~VertexPool() {
            glDeleteVertexArrays(1, &m_VAO);
            glDeleteBuffers(1, &m_VBO);
        }

        PoolAllocation allocate(const std::vector<T>& vertex);
        void free(const PoolAllocation& alloc);

        void bind() const { glBindVertexArray(m_VAO); }
        uint32_t capacity() const { return m_capacity; }
        uint32_t used() const { return m_used; }

      private:
        struct FreeBlock {
            uint32_t offset;
            uint32_t count;
            bool operator==(const FreeBlock& o) const {
                return offset == o.offset && count == o.count;
            }
            bool operator<(const FreeBlock& o) const { return offset < o.offset; }
        };

        void coalesce();


        GLuint m_VAO = 0;
        GLuint m_VBO = 0;
        uint32_t m_capacity;
        uint32_t m_used = 0;
        std::vector<FreeBlock> m_freeBlocks;
    };
}  // namespace gl

#include "VertexPool.hpp"