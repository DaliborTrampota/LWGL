#include "LWGL/texture/CubeMap.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/texture/ImageData.h"


#include <glad/glad.h>

using namespace gl;

void CubeMap::create(Settings setting) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    detail::ConfigureTexture(GL_TEXTURE_CUBE_MAP, setting);
}

void CubeMap::loadFace(CubeFace face, const gl::ImageData& data) {
    if (data.width != data.height)
        throw std::runtime_error("Cube map face must be square");

    m_width = data.width;
    m_channels = data.channels;

    detail::Data2D(detail::toGLCubeFace(face), data.width, data.height, data.format, data.data);
}

void CubeMap::loadFace(CubeFace face, const gl::RawImageData& rawImageData) {
    if (rawImageData.width != rawImageData.height)
        throw std::runtime_error("Cube map face must be square");

    m_width = rawImageData.width;
    m_channels = rawImageData.channels;

    glTexImage2D(
        detail::toGLCubeFace(face),
        0,
        rawImageData.internalFormat,
        rawImageData.width,
        rawImageData.height,
        0,
        rawImageData.format,
        rawImageData.dataType,
        rawImageData.data
    );
}


void CubeMap::loadFaceRaw(CubeFace face, int w, int ch, ImageFormat format, Data data) {
    if (w <= 0 || ch <= 0)
        throw std::runtime_error("Width and height must be greater than zero");

    m_width = w;
    m_channels = ch;

    detail::Data2D(
        detail::toGLCubeFace(face),
        w,
        w,  // Cube map faces are square
        format,
        data
    );
}
