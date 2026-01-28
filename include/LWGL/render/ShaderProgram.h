#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../Globals.h"

namespace gl {
    class UBO;
    class Shader;
    class TextureBase;

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

        ShaderProgram(ShaderProgram&&) noexcept;
        ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

        unsigned int id() const { return m_id; }
        void use() const;
        void bindUBO(const UBO& ubo) const;

        void bindTextures() const;
        void setTexture(unsigned int unit, const TextureBase* texture, const std::string& name);

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

        static void setConstant(const std::string& name, const std::string& value);
        static void setConstant(const std::string& name, float value);
        static void setConstant(const std::string& name, int value);
        static void setConstant(const std::string& name, bool value);


      protected:
        unsigned int m_id = 0;
        std::string m_name = "Unnamed";

        std::unordered_map<unsigned int, const TextureBase*> m_textureBindings;

      private:
        bool link();
    };

}  // namespace gl