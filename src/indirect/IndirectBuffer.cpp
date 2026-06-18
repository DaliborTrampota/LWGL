#include "LWGL/indirect/IndirectBuffer.h"

#include <LWGL/indirect/IndirectTypes.h>
#include <glad/glad.h>
#include <glm/glm.hpp>


using namespace gl;


IndirectBuffer::IndirectBuffer(size_t capacity) : m_gpuCapacity(capacity) {
    glCreateBuffers(1, &m_IB);
    glNamedBufferData(
        m_IB, m_gpuCapacity * sizeof(DrawArraysIndirectCommand), nullptr, GL_STREAM_DRAW
    );

    m_models.create(m_gpuCapacity);
    m_models.reserve(m_gpuCapacity);
    m_commands.reserve(m_gpuCapacity);
}

IndirectBuffer::~IndirectBuffer() {
    glDeleteBuffers(1, &m_IB);
}

void IndirectBuffer::begin() {
    m_commands.clear();
    m_models.clear();
    m_vertCount = 0;
}

void IndirectBuffer::add(PoolAllocation aloc, const glm::mat4& model) {
    DrawArraysIndirectCommand cmd{
        .count = aloc.count, .primCount = 1, .offset = aloc.offset, .baseInstance = 0
    };

    if (m_commands.size() >= m_gpuCapacity) {
        m_gpuCapacity += 50;
        m_commands.reserve(m_gpuCapacity);
        m_models.allocate(m_gpuCapacity);
        glNamedBufferData(
            m_IB, sizeof(DrawArraysIndirectCommand) * m_gpuCapacity, nullptr, GL_STREAM_DRAW
        );
    }
    m_models.add(model);
    m_commands.push_back(cmd);
    m_vertCount += aloc.count;
}
void IndirectBuffer::upload() {
    m_models.upload();
    glNamedBufferData(
        m_IB, sizeof(DrawArraysIndirectCommand) * m_commands.size(), nullptr, GL_STREAM_DRAW
    );
    glNamedBufferSubData(
        m_IB, 0, sizeof(DrawArraysIndirectCommand) * m_commands.size(), m_commands.data()
    );
}
void IndirectBuffer::draw() const {
    m_models.bind(m_modelBindPoint);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IB);
    glMultiDrawArraysIndirect(
        GL_TRIANGLES,
        nullptr,
        static_cast<GLsizei>(m_commands.size()),
        sizeof(DrawArraysIndirectCommand)
    );
}
