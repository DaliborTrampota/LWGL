#include "LWGL/texture/BindlessTextures.h"

#include <glad/glad.h>
#include <cassert>
#include <stdexcept>


#include "LWGL/texture/Texture2D.h"
#include "LWGL/texture/TextureRef.h"


using namespace gl;

BindlessTextures::BindlessTextures() {
    glCreateBuffers(1, &m_ssboID);
}

BindlessTextures::~BindlessTextures() {
    unload();
    if (m_ssboID != 0)
        glDeleteBuffers(1, &m_ssboID);
}

size_t BindlessTextures::add(Texture2D&& texture) {
    unsigned int id = texture.release();
    TextureRef ref(id, texture.type(), true);
    GLuint64 handle = glGetTextureHandleARB(id);

    m_textureHandles.push_back(handle);
    m_textures.emplace_back(std::move(ref));
    m_dirty = true;

    return m_textures.size() - 1;
}

size_t BindlessTextures::add(const TextureRef& texture) {
    GLuint64 handle = glGetTextureHandleARB(texture.id());

    m_textureHandles.push_back(handle);
    m_textures.push_back(texture);
    m_dirty = true;

    return m_textures.size() - 1;
}

void BindlessTextures::use() {
    for (GLuint64 handle : m_textureHandles)
        glMakeTextureHandleResidentARB(handle);
}

void BindlessTextures::use(size_t index) {
    glMakeTextureHandleResidentARB(m_textureHandles.at(index));
}

void BindlessTextures::unload() {
    for (GLuint64 handle : m_textureHandles)
        glMakeTextureHandleNonResidentARB(handle);
}

void BindlessTextures::unload(size_t index) {
    glMakeTextureHandleNonResidentARB(m_textureHandles.at(index));
}

void BindlessTextures::destroy(size_t index) {
    if (index >= m_textureHandles.size())
        throw std::out_of_range("BindlessTextures::destroy: index out of range");
    unload(index);
    m_textureHandles.erase(m_textureHandles.begin() + index);
    m_textures.erase(m_textures.begin() + index);
    m_dirty = true;
}

void BindlessTextures::bind(unsigned bindingPoint) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ssboID);
}

void BindlessTextures::unbind(unsigned bindingPoint) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, 0);
}

void BindlessTextures::setUniform(const char* name, size_t index, unsigned int programID) {
    GLint location = glGetUniformLocation(programID, name);
    assert(location != -1 && "setUniform: uniform not found in shader program");
    glProgramUniformHandleui64ARB(programID, location, m_textureHandles.at(index));
}

void BindlessTextures::update() {
    if (!m_dirty)
        return;
    glNamedBufferData(
        m_ssboID,
        sizeof(GLuint64) * m_textureHandles.size(),
        m_textureHandles.empty() ? nullptr : m_textureHandles.data(),
        GL_DYNAMIC_DRAW
    );
    m_dirty = false;
}
