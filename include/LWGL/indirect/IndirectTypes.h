#pragma once

#include "../GLTypes.h"

namespace gl {
    struct PoolAllocation {
        uint32_t offset = 0;
        uint32_t count = 0;

        bool isValid() const { return count > 0; }
    };


    struct DrawArraysIndirectCommand {
        GLuint count;         // vertex count
        GLuint primCount;     // always 1
        GLuint first;         // PoolAllocation::firstVertex
        GLuint baseInstance;  // always 0
    };

}  // namespace gl
