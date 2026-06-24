#include "LWGL/render/ComputeProgram.h"
#include "LWGL/Globals.h"


#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


#include "LWGL/render/Shader.h"

using namespace gl;

ComputeProgram::ComputeProgram(const std::filesystem::path& path) {
    GL_GUARD

    m_name = path.filename().string();
    Shader compute(path.string().c_str());
    if (compute.ID == 0) {
        printf("ComputeShaderError: Failed to compile shader program (%s)\n", m_name.c_str());
        return;
    }

    int success;
    char infoLog[512];

    m_ID = glCreateProgram();
    glAttachShader(m_ID, compute.ID);
    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        printf(
            "ComputeShaderError: Failed to link shader program (%s)\n%s", m_name.c_str(), infoLog
        );
        glDeleteProgram(m_ID);
        m_ID = 0;
        return;
    }
    printf("Compute shader compiled and linked\n");
}

ComputeProgram::~ComputeProgram() {
    GL_GUARD
    if (m_ID != 0) {
        glDeleteProgram(m_ID);
    }
}

void ComputeProgram::dispatch(uint32_t x, uint32_t y, uint32_t z) const {
    if (m_ID == 0) {
        printf("ComputeShaderError: dispatch() called on invalid program (%s)\n", m_name.c_str());
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