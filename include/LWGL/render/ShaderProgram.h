#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../Globals.h"

namespace gl {
    class UBO;
    class Shader;

    /// @brief ShaderProgram class representing shader program.
    class ShaderProgram {
      public:
        ShaderProgram() = delete;
        ShaderProgram(
            const std::string& vertexPath, const std::string& fragmentPath, std::string name
        );
        ShaderProgram(
            const std::string& vertexPath,
            const std::string& geometryPath,
            const std::string& fragmentPath,
            std::string name
        );
        ~ShaderProgram();

        // Prevent copying (ShaderProgram manages OpenGL resources)
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        // Allow moving if needed in the future
        ShaderProgram(ShaderProgram&&) noexcept = default;
        ShaderProgram& operator=(ShaderProgram&&) noexcept = default;

        void use() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        // template <typename... Shaders>
        // void attach(Shaders&&... shaders) {
        //     GL_GUARD

        //         (glAttachShader(m_id, shaders.ID), ...);
        // }

        void bindUBO(const UBO& ubo) const;

      protected:
      private:
        unsigned int m_id = 0;
        std::string m_name = "Unnamed";

        bool link();
    };

}  // namespace gl