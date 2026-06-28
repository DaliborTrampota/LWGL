#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>


#define GL_NONE 0

// OpenGL type aliases (standardized by Khronos, won't change)
// These match the official OpenGL specification
using GLenum = unsigned int;
using GLuint = unsigned int;
using GLint = int;
using GLsizei = int;
using GLuint64 = std::uint64_t;
using GLbitfield = unsigned int;


namespace gl {

    enum class TextureType {
        Texture1D,
        Texture2D,
        Texture3D,

        TextureArray1D,
        TextureArray,

        CubeMap,
        CubeMapArray,

        Multisample,
        MultisampleArray,

        Buffer
    };


    enum class CubeFace {
        Right = 0,
        Left,
        Top,
        Bottom,
        Back,
        Front
    };

    static constexpr std::array<glm::vec3, 6> CubeFaceDirections = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };

    static constexpr std::array<glm::vec3, 6> CubeFaceUps = {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f)
    };


    // enum class FBOAttachment : uint8_t {
    //     Depth,
    //     Stencil,
    //     DepthStencil,
    //     Color,
    // };

    // inline FBOAttachment operator>>(FBOAttachment attachment, int offset) {
    //     return static_cast<FBOAttachment>(static_cast<int>(attachment) + offset);
    // }

    // inline uint8_t operator+(FBOAttachment attachment, int offset) {
    //     return static_cast<int>(attachment) + offset;
    // }

    namespace FBOAttachment {
        using Attachment = uint8_t;
        constexpr Attachment Depth = 0;
        constexpr Attachment Stencil = 1;
        constexpr Attachment DepthStencil = 2;
        constexpr Attachment Color = 3;

        // Helper function to create color attachment indices
        // Usage: FBOAttachmentN::ColorN(0), FBOAttachmentN::ColorN(1), etc.
        constexpr Attachment ColorN(uint8_t offset) {
            return Color + offset;
        }
    };  // namespace FBOAttachment

}  // namespace gl