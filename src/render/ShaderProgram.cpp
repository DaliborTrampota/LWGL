#include "LWGL/render/ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <iterator>

#include "LWGL/buffer/UBO.h"
#include "LWGL/render/Shader.h"
#include "LWGL/texture/TextureBase.h"


using namespace gl;


gl::ShaderProgram::ShaderProgram(
    const char* vertexPath,
    const char* geometryPath,
    const char* fragmentPath,
    const char* name,
    bool deferCompilation
)
    : m_name(std::move(name)) {
    GL_GUARD
    m_id = glCreateProgram();

    m_shaderPaths[0] = vertexPath;
    m_shaderPaths[1] = geometryPath;
    m_shaderPaths[2] = fragmentPath;

    m_inUseBitmask = 0b111;

    if (!deferCompilation) {
        compile();
    }
}
gl::ShaderProgram::ShaderProgram(
    const char* vertexPath, const char* fragmentPath, const char* name, bool deferCompilation
)
    : m_name(std::move(name)) {
    GL_GUARD
    m_id = glCreateProgram();

    m_shaderPaths[0] = vertexPath;
    m_shaderPaths[1] = "";
    m_shaderPaths[2] = fragmentPath;

    m_inUseBitmask = 0b101;

    if (!deferCompilation) {
        compile();
    }
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
      m_textureBindings(std::move(other.m_textureBindings)),
      m_inUseBitmask(other.m_inUseBitmask),
      m_constants(std::move(other.m_constants)),
      m_compiled(other.m_compiled) {
    std::move(
        std::begin(other.m_shaderPaths), std::end(other.m_shaderPaths), std::begin(m_shaderPaths)
    );
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

void ShaderProgram::setGlobalConstant(const std::string& name, const char* value) {
    Shader::s_constants[name] = std::string(value);
}

void ShaderProgram::setGlobalConstant(const std::string& name, float value) {
    Shader::s_constants[name] = std::to_string(value);
}

void ShaderProgram::setGlobalConstant(const std::string& name, int value) {
    Shader::s_constants[name] = std::to_string(value);
}

void ShaderProgram::setGlobalConstant(const std::string& name, bool value) {
    Shader::s_constants[name] = value ? "true" : "false";
}

void ShaderProgram::setConstant(const std::string& name, const char* value) {
    m_constants[name] = std::string(value);
}

void ShaderProgram::setConstant(const std::string& name, float value) {
    m_constants[name] = std::to_string(value);
}

void ShaderProgram::setConstant(const std::string& name, int value) {
    m_constants[name] = std::to_string(value);
}

void ShaderProgram::setConstant(const std::string& name, bool value) {
    m_constants[name] = value ? "true" : "false";
}


bool ShaderProgram::link() {
    GL_GUARD
    glLinkProgram(m_id);

    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        printf("ShaderError: Failed to link shader program (%s)\n%s", m_name.c_str(), infoLog);
    }
    return success;
}

bool ShaderProgram::compile() {
    if (m_compiled) {
        printf("ShaderInfo: Recompiling shader program (%s)\n", m_name.c_str());
        GLint numShaders;
        glGetProgramiv(m_id, GL_ATTACHED_SHADERS, &numShaders);

        if (numShaders > 0) {
            // Get the shader IDs
            std::vector<GLuint> shaders(numShaders);
            glGetAttachedShaders(m_id, numShaders, nullptr, shaders.data());

            for (GLuint ID : shaders) {
                glDetachShader(m_id, ID);
            }
        }
    }

    Shader::Symbols symbols = {
        .programName = m_name,
    };
    std::vector<Shader*> shaders;

    for (uint8_t i = 0; i < m_inUseBitmask; i++) {
        if (m_inUseBitmask & (1 << i)) {
            ShaderType type = static_cast<ShaderType>(i);
            Shader* shader = new Shader(m_shaderPaths[i].c_str(), type, symbols, m_constants);
            shaders.push_back(shader);
            if (shader->ID == 0) {
                for (auto& shader : shaders) {
                    delete shader;
                }
                return false;
            }
            glAttachShader(m_id, shader->ID);
        }
    }

    m_compiled = link();

    for (auto& shader : shaders) {
        delete shader;
    }
    return m_compiled;
}