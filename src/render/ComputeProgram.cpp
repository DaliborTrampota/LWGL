#include "LWGL/render/ComputeProgram.h"
#include "LWGL/Globals.h"


#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


#include "LWGL/render/Shader.h"

using namespace gl;

ComputeProgram::ComputeProgram(const std::filesystem::path& path, bool deferCompilation) {
    GL_GUARD

    m_name = path.filename().string();
    m_path = path.string();
    m_ID = glCreateProgram();

    if (!deferCompilation) {
        compile();
    }
}

ComputeProgram::~ComputeProgram() {
    GL_GUARD
    if (m_ID != 0) {
        glDeleteProgram(m_ID);
    }
}

bool ComputeProgram::compile() {
    GL_GUARD

    GLint numShaders;
    glGetProgramiv(m_ID, GL_ATTACHED_SHADERS, &numShaders);
    if (numShaders > 0) {
        std::vector<GLuint> shaders(numShaders);
        glGetAttachedShaders(m_ID, numShaders, nullptr, shaders.data());
        for (GLuint id : shaders) {
            glDetachShader(m_ID, id);
        }
    }

    Shader::Symbols symbols = {.programName = m_name, .shaderType = "compute"};
    Shader shader(m_path.c_str(), ShaderType::Compute, symbols, m_constants);
    if (shader.ID == 0) {
        printf("ComputeShaderError: Failed to compile shader (%s)\n", m_name.c_str());
        return false;
    }

    glAttachShader(m_ID, shader.ID);
    glLinkProgram(m_ID);

    int success;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        printf(
            "ComputeShaderError: Failed to link shader program (%s)\n%s", m_name.c_str(), infoLog
        );
        m_compiled = false;
        return false;
    }

    printf("Compute shader compiled and linked\n");
    m_compiled = true;
    return true;
}

void ComputeProgram::setConstant(const std::string& name, const char* value) {
    m_constants[name] = std::string(value);
}

void ComputeProgram::setConstant(const std::string& name, float value) {
    m_constants[name] = std::to_string(value);
}

void ComputeProgram::setConstant(const std::string& name, int value) {
    m_constants[name] = std::to_string(value);
}

void ComputeProgram::setConstant(const std::string& name, bool value) {
    m_constants[name] = value ? "true" : "false";
}

void ComputeProgram::dispatch(uint32_t x, uint32_t y, uint32_t z) const {
    if (m_ID == 0) {
        printf("ComputeShaderError: dispatch() called on invalid program (%s)\n", m_name.c_str());
        return;
    }

    if (!m_compiled) {
        printf(
            "ComputeShaderError: dispatch() called on uncompiled program (%s)\n", m_name.c_str()
        );
        return;
    }

    glUseProgram(m_ID);
    glDispatchCompute(x, y, z);
}


void ComputeProgram::setBool(const std::string& name, bool value) const {
    glProgramUniform1i(m_ID, glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void ComputeProgram::setInt(const std::string& name, int value) const {
    glProgramUniform1i(m_ID, glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeProgram::setUInt(const std::string& name, uint32_t value) const {
    glProgramUniform1ui(m_ID, glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeProgram::setFloat(const std::string& name, float value) const {
    glProgramUniform1f(m_ID, glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    glProgramUniform2fv(m_ID, glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void ComputeProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glProgramUniform3fv(m_ID, glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void ComputeProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    glProgramUniform4fv(m_ID, glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void ComputeProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glProgramUniformMatrix4fv(
        m_ID, glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)
    );
}
