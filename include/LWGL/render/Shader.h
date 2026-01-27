#pragma once

#include <glad/glad.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace gl {

    enum class ShaderType {
        Vertex = 0,
        Geometry = 1,
        Fragment = 2,


        Compute,
    };

    struct Shader {
        static void setChunksDirectory(fs::path directory) { s_chunksDirectory = directory; }

        unsigned int ID;

        Shader(const char* path, ShaderType type);
        ~Shader();

        Shader(Shader&& other) noexcept = delete;
        Shader& operator=(Shader&& other) noexcept = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        bool compile(std::string source) const;

      protected:
        static inline fs::path s_chunksDirectory;
    };
}  // namespace gl
