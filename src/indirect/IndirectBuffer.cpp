#include "LWGL/indirect/IndirectBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>



using namespace gl;


IndirectBuffer::IndirectBuffer() {
    glGenBuffers(1, &m_IB);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IB);
    glNamedBufferData(m_IB, sizeof(DrawArraysIndirectCommand), nullptr, GL_STREAM_DRAW);

    m_models.create();
}

IndirectBuffer::~IndirectBuffer() {
    glDeleteBuffers(1, &m_IB);
}

void IndirectBuffer::begin() {
    m_commands.clear();
    m_models.clear();
    m_vertCount = 0;
}

void IndirectBuffer::add(PoolAllocation aloc, glm::mat4 model) {
    DrawArraysIndirectCommand cmd{
        .count = aloc.count, .primCount = 1, .first = aloc.offset, .baseInstance = 0
    };
    m_models.add(model);
    m_commands.push_back(cmd);
    m_vertCount += aloc.count;
}
void IndirectBuffer::upload() {
    m_models.upload();
    glNamedBufferData(
        m_IB,
        sizeof(DrawArraysIndirectCommand) * m_commands.size(),
        m_commands.data(),
        GL_STREAM_DRAW
    );
}
void IndirectBuffer::draw() const {
    m_models.bind(m_modelBindPoint);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IB);
    glMultiDrawArraysIndirect(
        GL_TRIANGLES, (void*)0, m_commands.size(), sizeof(DrawArraysIndirectCommand)
    );
}
