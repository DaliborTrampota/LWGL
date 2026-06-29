#pragma once

#include <cstdint>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>


namespace gl {
    class ComputeProgram {
      public:
        ComputeProgram(const std::filesystem::path& path, bool deferCompilation = false);
        ~ComputeProgram();

        unsigned int id() const { return m_ID; }

        bool compile();

        void setConstant(const std::string& name, const char* value);
        void setConstant(const std::string& name, float value);
        void setConstant(const std::string& name, int value);
        void setConstant(const std::string& name, bool value);

        void dispatch(uint32_t x, uint32_t y, uint32_t z) const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setUInt(const std::string& name, uint32_t value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

      private:
        unsigned int m_ID = 0;
        std::string m_name;
        std::string m_path;
        std::unordered_map<std::string, std::string> m_constants;
        bool m_compiled = false;
    };
}  // namespace gl
