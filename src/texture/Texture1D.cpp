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
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_1D, m_id);

    detail::ConfigureTexture(GL_TEXTURE_1D, settings);
}

void Texture1D::load(const gl::ImageData& imageData) {
    m_width = imageData.width;
    m_channels = imageData.channels;

    detail::Data1D(GL_TEXTURE_1D, imageData.width, imageData.format, imageData.data);
}

void Texture1D::loadRaw(int w, int ch, gl::ImageFormat format, Data data) {
    m_width = w;
    m_channels = ch;

    detail::Data1D(GL_TEXTURE_1D, w, format, data);
}