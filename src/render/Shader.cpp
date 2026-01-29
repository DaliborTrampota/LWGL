#include "LWGL/render/Shader.h"
#include "LWGL/Globals.h"

#include <fstream>
#include <string>
#include <unordered_map>

using namespace gl;

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

Shader::Shader(const char* path, ShaderType type) {
    GL_GUARD
    ID = glCreateShader(shaderTypeToGL(type));

    std::string content = readFile(path);
    if (content.empty()) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s\n", path);
        return;
    }

    const char* code = content.c_str();
    if (!compile(content)) {
        printf("ERROR::SHADER::FAILED_TO_COMPILE: %s\n", path);
        return;
    }


    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED::%s\n%s", path, infoLog);
    }
    printf("Shader %s compiled %d\n", path, ID);
}

Shader::~Shader() {
    GL_GUARD
    glDeleteShader(ID);
}

bool Shader::compile(std::string& source) const {
    static std::string s_start = "{{";
    static std::string s_end = "}}";

    size_t lastFind = 0;
    bool findEndTag = false;
    while (true) {
        if (findEndTag) {
            size_t endTag = source.find(s_end, lastFind);
            if (endTag == std::string::npos) {
                printf("ERROR::SHADER::FAILED_TO_FIND_END_TAG\n");
                return false;
            }
            size_t tagLength = endTag - lastFind - s_start.length();
            std::string tag = source.substr(lastFind + s_start.length(), tagLength);
            tag = trim(tag);

            size_t replaceLength = endTag - lastFind + s_end.length();
            if (s_chunks.find(tag) != s_chunks.end()) {
                source.replace(lastFind, replaceLength, s_chunks[tag]);
            } else if (s_constants.find(tag) != s_constants.end()) {
                source.replace(lastFind, replaceLength, s_constants[tag]);
            } else {
                printf("ERROR::SHADER::FAILED_TO_FIND_CHUNK: %s\n", tag.c_str());
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