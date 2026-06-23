#include "LWGL/render/Shader.h"
#include "LWGL/Globals.h"

#include <fstream>
#include <string>
#include <unordered_map>

using namespace gl;
namespace fs = std::filesystem;

namespace {
    constexpr GLenum shaderTypeToGL(ShaderType type) {
        switch (type) {
            case ShaderType::Vertex: return GL_VERTEX_SHADER;
            case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
            case ShaderType::Compute: return GL_COMPUTE_SHADER;
            default: return GL_NONE;  // Invalid type
        }
    }

    const char* ws = " \t\n\r\f\v";

    // trim from end of string (right)
    inline std::string& rtrim(std::string& s, const char* t = ws) {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning of string (left)
    inline std::string& ltrim(std::string& s, const char* t = ws) {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from both ends of string (right then left)
    inline std::string& trim(std::string& s, const char* t = ws) {
        return ltrim(rtrim(s, t), t);
    }

    std::string readFile(const char* path) {
        std::ifstream R(path);
        if (!R.is_open()) {
            return "";
        }
        const std::string content(
            (std::istreambuf_iterator<char>(R)), std::istreambuf_iterator<char>()
        );
        R.close();
        return content;
    }
}  // namespace

void Shader::setChunksDirectory(fs::path directory) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        printf("ERROR::SHADER::CHUNKS_DIRECTORY_NOT_FOUND: %s\n", directory.string().c_str());
        return;
    }
    s_chunksDirectory = directory;
    for (const auto& entry : fs::directory_iterator(s_chunksDirectory)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename().string();
            // if (!s_chunks.contains(name)) {
            s_chunks[name] = readFile(entry.path().string().c_str());
            // }
        }
    }
}

Shader::Shader(const char* computeShaderPath) {
    GL_GUARD
    m_type = ShaderType::Compute;
    std::string content = readFile(computeShaderPath);
    if (content.empty()) {
        printf("ShaderError: ComputeShader source is empty (%s)\n", computeShaderPath);
        return;
    }

    compile(content);
}

Shader::Shader(const char* path, ShaderType type, Symbols symbols, Constants constants)
    : m_path(path),
      m_symbols(std::move(symbols)),
      m_constants(std::move(constants)),
      m_type(type) {
    GL_GUARD

    switch (type) {
        case ShaderType::Vertex: m_symbols.shaderType = "vertex"; break;
        case ShaderType::Geometry: m_symbols.shaderType = "geometry"; break;
        case ShaderType::Fragment: m_symbols.shaderType = "fragment"; break;
        case ShaderType::Compute: m_symbols.shaderType = "compute"; break;
    }

    std::string content = readFile(path);
    if (content.empty()) {
        printf("ShaderError: Shader source is empty (%s)\n", path);
        return;
    }

    compile(content);
}

Shader::~Shader() {
    GL_GUARD
    if (ID != 0) {
        glDeleteShader(ID);
    }
}

bool Shader::compile(std::string& source, Constants localConstants) const {
    static std::string s_start = "{{";
    static std::string s_end = "}}";

    size_t lastFind = 0;
    bool findEndTag = false;
    while (true) {
        if (findEndTag) {
            size_t endTag = source.find(s_end, lastFind);
            if (endTag == std::string::npos) {
                printf("ShaderError: Failed to find end tag\n");
                return false;
            }
            size_t tagLength = endTag - lastFind - s_start.length();
            std::string tag = source.substr(lastFind + s_start.length(), tagLength);
            tag = trim(tag);
            unrollSymbols(tag);

            size_t replaceLength = endTag - lastFind + s_end.length();
            if (localConstants.find(tag) != localConstants.end()) {
                source.replace(lastFind, replaceLength, localConstants[tag]);
            } else if (s_chunks.find(tag) != s_chunks.end()) {
                source.replace(lastFind, replaceLength, s_chunks[tag]);
            } else if (s_constants.find(tag) != s_constants.end()) {
                source.replace(lastFind, replaceLength, s_constants[tag]);
            } else {
                printf("ShaderError: Failed to find chunk or constant (%s)\n", tag.c_str());
                return false;
            }
            findEndTag = false;
        } else {
            lastFind = source.find(s_start, lastFind);
            if (lastFind == std::string::npos) {
                break;
            }
            findEndTag = true;
        }
    }

    const char* code = source.c_str();
    glShaderSource(ID, 1, &code, NULL);
    glCompileShader(ID);
    return true;
}

void Shader::unrollSymbols(std::string& tag) const {
    // {{tag}} -> tag
    // {{#name.tag}} -> program_name.tag
    // {{#type.tag}} -> shader_type.tag

    if (!tag.starts_with("#"))
        return;

    size_t dotPos = tag.find('.');
    if (dotPos == std::string::npos)
        return;

    std::string symbol = tag.substr(1, dotPos - 1);

    if (symbol == "name") {
        tag.replace(0, dotPos + 1, m_symbols.programName);

    } else if (symbol == "type") {
        tag.replace(0, dotPos + 1, m_symbols.shaderType);
    }
}

void Shader::compile(std::string& content) {
    ID = glCreateShader(shaderTypeToGL(m_type));
    if (!compile(content, m_constants)) {
        glDeleteShader(ID);
        ID = 0;
        printf("ShaderError: Failed to preprocess shader (%s)\n", m_path.c_str());
        return;
    }

    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        glDeleteShader(ID);
        ID = 0;
        printf("ShaderError: Failed to compile shader (%s)\n%s", m_path.c_str(), infoLog);
        return;
    }
    printf("Shader %s %s compiled %d\n", m_symbols.shaderType.c_str(), m_path.c_str(), ID);
}