#include "LWGL/render/ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include "LWGL/buffer/UBO.h"
#include "LWGL/render/Shader.h"
#include "LWGL/texture/TextureBase.h"


using namespace gl;


gl::ShaderProgram::ShaderProgram(
    const std::string& vertexPath,
    const std::string& geometryPath,
    const std::string& fragmentPath,
    std::string name
)
    : m_name(std::move(name)) {
    GL_GUARD
    m_id = glCreateProgram();

    Shader vert(vertexPath.c_str(), ShaderType::Vertex);
    Shader geom(geometryPath.c_str(), ShaderType::Geometry);
    Shader frag(fragmentPath.c_str(), ShaderType::Fragment);

    glAttachShader(m_id, vert.ID);
    glAttachShader(m_id, geom.ID);
    glAttachShader(m_id, frag.ID);
    link();
}
gl::ShaderProgram::ShaderProgram(
    const std::string& vertexPath, const std::string& fragmentPath, std::string name
)
    : m_name(std::move(name)) {
    GL_GUARD
    m_id = glCreateProgram();

    Shader vert(vertexPath.c_str(), ShaderType::Vertex);
    Shader frag(fragmentPath.c_str(), ShaderType::Fragment);

    glAttachShader(m_id, vert.ID);
    glAttachShader(m_id, frag.ID);
    link();
}

ShaderProgram::~ShaderProgram() {
    GL_GUARD
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
}


ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id(other.m_id),
      m_name(std::move(other.m_name)),
      m_textureBindings(std::move(other.m_textureBindings)) {
    other.m_id = 0;
}

void ShaderProgram::use() const {
    GL_GUARD
    glUseProgram(m_id);
}

void ShaderProgram::bindTextures() const {
    for (const auto& [unit, texture] : m_textureBindings) {
        texture->activate(unit);
    }
}

void ShaderProgram::setTexture(
    unsigned int unit, const TextureBase* texture, const std::string& name
) {
    m_textureBindings[unit] = texture;
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), unit);
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::bindUBO(const UBO& ubo) const {
    ubo.bindToProgram(m_id);
}


bool ShaderProgram::link() {
    GL_GUARD
    glLinkProgram(m_id);

    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM\n%s", infoLog);
    }
    return success;
}