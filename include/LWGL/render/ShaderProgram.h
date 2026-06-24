#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>


namespace gl {
    class UBO;
    class Shader;
    class TextureBase;

    /// @brief ShaderProgram class representing shader program.
    class ShaderProgram {
      public:
        ShaderProgram() = delete;
        ShaderProgram(
            const char* vertexPath,
            const char* fragmentPath,
            const char* name,
            bool deferCompilation = false
        );
        ShaderProgram(
            const char* vertexPath,
            const char* geometryPath,
            const char* fragmentPath,
            const char* name,
            bool deferCompilation = false
        );
        ~ShaderProgram();

        // Prevent copying (ShaderProgram manages OpenGL resources)
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        ShaderProgram(ShaderProgram&&) noexcept;
        ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

        /// @brief If constructed with deferCompilation, this will compile the program.
        /// @note If the program is already compiled, it will be recompiled.
        bool compile();

        unsigned int id() const { return m_id; }
        void use() const;
        void bindUBO(const UBO& ubo) const;

        void bindTextures() const;
        void setTexture(unsigned int unit, const TextureBase* texture, const std::string& name);

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setUInt(const std::string& name, uint32_t value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        static void setGlobalConstant(const std::string& name, const char* value);
        static void setGlobalConstant(const std::string& name, float value);
        static void setGlobalConstant(const std::string& name, int value);
        static void setGlobalConstant(const std::string& name, bool value);

        void setConstant(const std::string& name, const char* value);
        void setConstant(const std::string& name, float value);
        void setConstant(const std::string& name, int value);
        void setConstant(const std::string& name, bool value);


      protected:
        unsigned int m_id = 0;
        std::string m_name;

        std::unordered_map<unsigned int, const TextureBase*> m_textureBindings;
        std::unordered_map<std::string, std::string> m_constants;

        std::uint8_t m_inUseBitmask = 0;
        std::string m_shaderPaths[3];
        bool m_compiled = false;

      private:
        bool link();
    };

}  // namespace gl