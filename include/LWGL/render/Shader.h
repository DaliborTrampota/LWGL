#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

namespace gl {

    enum class ShaderType {
        Vertex = 0,
        Geometry = 1,
        Fragment = 2,


        Compute,
    };

    struct Shader {
        static void setChunksDirectory(fs::path directory);

        unsigned int ID;

        Shader(const char* path, ShaderType type);
        ~Shader();

        Shader(Shader&& other) noexcept = delete;
        Shader& operator=(Shader&& other) noexcept = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        bool compile(std::string& source) const;

      protected:
        friend class ShaderProgram;
        static inline fs::path s_chunksDirectory;
        static inline std::unordered_map<std::string, std::string> s_constants;
        static inline std::unordered_map<std::string, std::string> s_chunks;
    };
}  // namespace gl
