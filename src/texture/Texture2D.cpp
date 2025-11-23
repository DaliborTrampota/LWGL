#include "LWGL/texture/Texture2D.h"
#include "../detail/TexturePrivate.h"
#include "LWGL/texture/ImageData.h"


#include <glad/glad.h>
#include <stdexcept>

using namespace gl;


void Texture2D::create(Settings settings) {
    if (m_id != 0)
        throw std::runtime_error("Texture2D already created");

    glGenTextures(1, &m_id);
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_2D, m_id);

    detail::ConfigureTexture(GL_TEXTURE_2D, settings);
}

void Texture2D::load(const gl::ImageData& imageData) {
    m_width = imageData.width;
    m_height = imageData.height;
    m_channels = imageData.channels;

    detail::Data2D(
        GL_TEXTURE_2D, imageData.width, imageData.height, imageData.format, imageData.data
    );
}

void Texture2D::loadRaw(int w, int h, int ch, gl::ImageFormat format, Data data) {
    m_width = w;
    m_height = h;
    m_channels = ch;

    detail::Data2D(GL_TEXTURE_2D, w, h, format, data);
}

void Texture2D::update(gl::ImageFormat format, Data data) {
    detail::SubData2D(GL_TEXTURE_2D, 0, 0, m_width, m_height, format, data);
}