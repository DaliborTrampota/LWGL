#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <string>
#include <unordered_map>

namespace gl {

    enum class ShaderType {
        Vertex = 0,
        Geometry = 1,
        Fragment = 2,


        Compute,
    };

    struct Shader {
      private:
        using Constants = std::unordered_map<std::string, std::string>;

      public:
        static void setChunksDirectory(std::filesystem::path directory);

        unsigned int ID = 0;

        Shader(const char* computeShaderPath);
        ~Shader();

        Shader(Shader&& other) noexcept = delete;
        Shader& operator=(Shader&& other) noexcept = delete;
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

        bool compile(std::string& source, Constants localConstants) const;

      protected:
        friend class ShaderProgram;
        friend class ComputeProgram;

        struct Symbols {
            std::string programName;
            std::string shaderType;
        };

        Shader(const char* path, ShaderType type, Symbols symbols, Constants constants);

        static inline std::filesystem::path s_chunksDirectory;
        static inline std::unordered_map<std::string, std::string> s_constants;
        static inline std::unordered_map<std::string, std::string> s_chunks;

        void unrollSymbols(std::string& tag) const;
        void compile(std::string& content);

        std::string m_path;
        Constants m_constants;
        Symbols m_symbols;
        ShaderType m_type;
    };
}  // namespace gl
