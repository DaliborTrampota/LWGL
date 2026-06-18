#include "VertexPool.h"

#include <glad/glad.h>
#include <algorithm>
#include <cstdio>


namespace gl {
    template <gl::VertexType T>
    VertexPool<T>::VertexPool(uint32_t size) : m_capacity(size) {
        glCreateVertexArrays(1, &m_VAO);
        glCreateBuffers(1, &m_VBO);
        glNamedBufferStorage(m_VBO, size * sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);


        VertexLayout layout = T::layout();
        glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, layout.stride);
        for (auto& attr : layout.attributes) {
            glEnableVertexArrayAttrib(m_VAO, attr.location);
            glVertexArrayAttribBinding(m_VAO, attr.location, 0);
            if (attr.type == VertexAttribute::Float) {
                glVertexArrayAttribFormat(
                    m_VAO, attr.location, attr.size, GL_FLOAT, GL_FALSE, attr.offset
                );
            } else {
                GLenum glType = attr.type == VertexAttribute::Int ? GL_INT : GL_UNSIGNED_INT;
                glVertexArrayAttribIFormat(m_VAO, attr.location, attr.size, glType, attr.offset);
            }
        }

        m_freeBlocks.push_back({0, size});
    }


    template <gl::VertexType T>
    PoolAllocation VertexPool<T>::allocate(std::vector<T>& data) {
        if (data.empty()) {
            return {0, 0};
        }
        for (FreeBlock& block : m_freeBlocks) {
            if (block.count >= data.size()) {
                glNamedBufferSubData(
                    m_VBO, block.offset * sizeof(T), data.size() * sizeof(T), data.data()
                );

                PoolAllocation alloc{block.offset, data.size()};

                if (block.count == data.size()) {
                    std::erase(m_freeBlocks, block);
                } else {
                    block.offset += data.size();
                    block.count -= data.size();
                }
                m_used += alloc.count;
                return alloc;
            }
        }
        printf("VertexPool: Allocation failed\n");
        return {0, 0};
    }


    template <gl::VertexType T>
    void VertexPool<T>::free(const PoolAllocation& alloc) {
        m_used -= alloc.count;
        FreeBlock block{alloc.offset, alloc.count};
        m_freeBlocks.push_back(block);
        coalesce();
    }

    template <gl::VertexType T>
    void VertexPool<T>::coalesce() {
        std::sort(m_freeBlocks.begin(), m_freeBlocks.end());
        for (int i = m_freeBlocks.size() - 1; i > 0; i--) {
            auto& prev = m_freeBlocks[i - 1];
            auto& cur = m_freeBlocks[i];
            if (prev.offset + prev.count == cur.offset) {
                prev.count += cur.count;
                m_freeBlocks.erase(m_freeBlocks.begin() + i);
            }
        }
    }
}  // namespace gl
