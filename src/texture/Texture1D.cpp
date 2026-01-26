#include "LWGL/texture/Texture1D.h"
#include "../detail/TexturePrivate.h"
#include "LWGL/texture/ImageData.h"


#include <glad/glad.h>
#include <stdexcept>


using namespace gl;


void Texture1D::create(TextureParams params) {
    if (m_id != 0)
        throw std::runtime_error("Texture1D already created");

    if (m_immutable) {
        glCreateTextures(GL_TEXTURE_1D, 1, &m_id);
        detail::ConfigureTexture(m_id, params);
    } else {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, m_id);
        detail::ConfigureTexture(m_id, params);
    }
}

Texture1D Texture1D::fromImageData(const gl::ImageData& imageData, TextureParams params) {
    Texture1D texture;
    texture.m_channels = imageData.channels;
    texture.create(params);
    texture.allocate({
        .width = imageData.width,
        .format = imageData.format,
        .dataType = imageData.dataType,
    });
    texture.upload(imageData.format, imageData.data);

    return texture;
}

Texture1D Texture1D::fromRawData(const gl::RawImageData& rawImageData, TextureParams params) {
    Texture1D texture;
    texture.create(params);
    texture.m_width = rawImageData.width;
    texture.m_channels = rawImageData.channels;


    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_1D, texture.m_id);

    // glTexImage1D(
    //     GL_TEXTURE_1D,
    //     0,
    //     rawImageData.internalFormat,
    //     rawImageData.width,
    //     0,
    //     rawImageData.format,
    //     rawImageData.dataType,
    //     rawImageData.data
    // );
    glTextureStorage1D(texture.m_id, 0, rawImageData.internalFormat, texture.m_width);

    glTextureSubImage1D(
        texture.m_id,
        0,
        0,
        texture.m_width,
        rawImageData.format,
        rawImageData.dataType,
        rawImageData.data
    );
    return texture;
}

void Texture1D::allocate(TextureStorage storage) {
    if (m_id == 0)
        throw std::runtime_error("Texture2D not created");

    m_width = storage.width;
    // m_channels = storage.channels; // TODO deduce from format?

    if (m_immutable) {
        detail::Data1DImmutable(m_id, m_width, storage.format);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, m_id);
        detail::Data1D(GL_TEXTURE_1D, m_width, storage.format, nullptr, storage.dataType);
    }
}

void Texture1D::upload(ImageFormat format, Data data) {
    if (m_id == 0)
        throw std::runtime_error("Texture2D not created");

    detail::SubData1D(m_id, 0, m_width, format, data);
}