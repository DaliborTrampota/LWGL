#include "LWGL/texture/CubeMap.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/texture/ImageData.h"


#include <glad/glad.h>

using namespace gl;

CubeMap CubeMap::fromImageData(ImageData imageData[6], TextureParams params, bool immutable) {
    CubeMap cubeMap(immutable);
    cubeMap.create(params);
    cubeMap.allocate({
        .width = imageData[0].width,
        .height = imageData[0].height,
        .format = imageData[0].format,
        .dataType = imageData[0].dataType,
    });

    for (int i = 0; i < 6; i++) {
        cubeMap.upload(static_cast<CubeFace>(i), imageData[i]);
    }
    return cubeMap;
}

void CubeMap::create(TextureParams params) {
    if (m_id != 0)
        throw std::runtime_error("CubeMap already created");

    if (m_immutable) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);
    } else {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    }
    detail::ConfigureTexture(m_id, params);
}

void CubeMap::allocate(TextureStorage storage) {
    if (m_id == 0)
        throw std::runtime_error("CubeMap not created");

    m_width = storage.width;

    if (m_immutable) {
        detail::Data2DImmutable(m_id, m_width, m_width, storage.format);
    }
}

void CubeMap::upload(CubeFace face, const gl::ImageData& data) {
    if (m_id == 0)
        throw std::runtime_error("CubeMap not created");

    if (data.width != data.height)
        throw std::runtime_error("Cube map face must be square");

    // TODO can upload smaller width, track somehow?
    m_channels = data.channels;

    if (m_immutable) {
        detail::SubData3D(
            m_id,
            0,
            0,
            static_cast<int>(face),
            data.width,
            data.width,
            1,
            data.format,
            data.data,
            data.dataType
        );
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        detail::Data2D(
            detail::toGLCubeFace(face), data.width, data.width, data.format, data.data, data.dataType
        );
    }
}

void CubeMap::upload(CubeFace face, const gl::RawImageData& rawImageData) {
    if (m_id == 0)
        throw std::runtime_error("CubeMap not created");

    if (rawImageData.width != rawImageData.height)
        throw std::runtime_error("Cube map face must be square");

    m_channels = rawImageData.channels;

    if (m_immutable) {
        glTextureSubImage3D(
            m_id,
            0,
            0,
            0,
            static_cast<int>(face),
            rawImageData.width,
            rawImageData.width,
            1,
            rawImageData.format,
            rawImageData.dataType,
            rawImageData.data
        );
    } else {
        glTexImage2D(
            detail::toGLCubeFace(face),
            0,
            rawImageData.internalFormat,
            rawImageData.width,
            rawImageData.width,
            0,
            rawImageData.format,
            rawImageData.dataType,
            rawImageData.data
        );
    }
}


void CubeMap::upload(CubeFace face, int width, ImageFormat format, Data data) {
    if (m_id == 0)
        throw std::runtime_error("CubeMap not created");
    if (width <= 0)
        throw std::runtime_error("Width must be greater than zero");

    // m_channels = ch;

    if (m_immutable) {
        detail::SubData3D(m_id, 0, 0, static_cast<int>(face), width, width, 1, format, data);
    } else {
        detail::Data2D(
            detail::toGLCubeFace(face),
            width,
            width,  // Cube map faces are square
            format,
            data
        );
    }
}
