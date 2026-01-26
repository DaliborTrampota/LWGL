#include "LWGL/texture/Texture2D.h"
#include "../detail/TexturePrivate.h"
#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/TextureBase.h"

#include <glad/glad.h>
#include <stdexcept>

using namespace gl;


Texture2D Texture2D::fromImageData(
    const gl::ImageData& imageData, TextureParams params, bool immutable
) {
    Texture2D texture(immutable);
    texture.m_channels = imageData.channels;

    texture.create(params);
    texture.allocate({
        .width = imageData.width,
        .height = imageData.height,
        .format = imageData.format,
        .dataType = imageData.dataType,
    });
    texture.upload(imageData.format, imageData.data);
    return texture;
}

Texture2D Texture2D::fromRawData(
    const gl::RawImageData& rawImageData, TextureParams params, bool immutable
) {
    Texture2D texture(immutable);
    texture.create(params);
    texture.m_width = rawImageData.width;
    texture.m_height = rawImageData.height;
    texture.m_channels = rawImageData.channels;

    if (immutable) {
        glTextureStorage2D(
            texture.m_id, 0, rawImageData.internalFormat, texture.m_width, texture.m_height
        );

        glTextureSubImage2D(
            texture.m_id,
            0,
            0,
            0,
            texture.m_width,
            texture.m_height,
            rawImageData.format,
            rawImageData.dataType,
            rawImageData.data
        );
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);

        glTexImage2D(
            GL_TEXTURE_2D,
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
    return texture;
}

Texture2D Texture2D::forRenderTarget(TextureStorage storage, TextureParams params, bool immutable) {
    Texture2D texture(immutable);
    texture.create(params);
    texture.allocate(storage);
    return texture;
}

void Texture2D::create(TextureParams params) {
    if (m_id != 0)
        throw std::runtime_error("Texture2D already created");

    if (m_immutable) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        detail::ConfigureTexture(m_id, params);
    } else {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        detail::ConfigureTexture(m_id, params);
    }
}

void Texture2D::allocate(TextureStorage storage) {
    if (m_id == 0)
        throw std::runtime_error("Texture2D not created");

    m_width = storage.width;
    m_height = storage.height;
    // m_channels = 0;  // TODO deduce from format?

    if (m_immutable) {
        detail::Data2DImmutable(m_id, m_width, m_height, storage.format);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        detail::Data2D(GL_TEXTURE_2D, m_width, m_height, storage.format, nullptr, storage.dataType);
    }
}

void Texture2D::upload(ImageFormat format, Data data) {
    if (m_id == 0)
        throw std::runtime_error("Texture2D not created");

    detail::SubData2D(m_id, 0, 0, m_width, m_height, format, data);
}