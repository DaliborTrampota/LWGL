#include "LWGL/texture/Texture1D.h"
#include "../detail/TexturePrivate.h"
#include "LWGL/texture/ImageData.h"


#include <glad/glad.h>
#include <stdexcept>


using namespace gl;


void Texture1D::create(Settings settings) {
    if (m_id != 0)
        throw std::runtime_error("Texture1D already created");

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_1D, m_id);

    detail::ConfigureTexture(GL_TEXTURE_1D, settings);
}

void Texture1D::load(const gl::ImageData& imageData) {
    m_width = imageData.width;
    m_channels = imageData.channels;

    detail::Data1D(
        GL_TEXTURE_1D, imageData.width, imageData.format, imageData.data, imageData.dataType
    );
}

void Texture1D::load(const gl::RawImageData& rawImageData) {
    m_width = rawImageData.width;
    m_channels = rawImageData.channels;

    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        rawImageData.internalFormat,
        rawImageData.width,
        0,
        rawImageData.format,
        rawImageData.dataType,
        rawImageData.data
    );
}

void Texture1D::loadRaw(int w, int ch, ImageFormat format, Data data) {
    m_width = w;
    m_channels = ch;

    detail::Data1D(GL_TEXTURE_1D, w, format, data);
}