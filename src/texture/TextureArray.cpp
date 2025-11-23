#include "LWGL/texture/TextureArray.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/texture/ImageData.h"


#include <format>
#include <stdexcept>

#include <glad/glad.h>
#include <tools/stb_image.h>

using namespace gl;


void TextureArray::create(ArraySettings setting) {
    glGenTextures(1, &m_id);
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

    detail::ConfigureTexture(GL_TEXTURE_2D_ARRAY, setting);

    m_maxLayers = setting.layers;
    m_format = detail::toGLFormat(setting.format);
    m_width = setting.width;
    m_height = setting.height;

    detail::Data3D(
        GL_TEXTURE_2D_ARRAY,
        setting.width,   // width of each 2D layer
        setting.height,  // height of each 2D layer
        setting.layers,  // number of layers
        setting.format,
        nullptr
    );
}

void TextureArray::create(RawArraySettings settings) {
    glGenTextures(1, &m_id);
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

    detail::ConfigureTexture(GL_TEXTURE_2D_ARRAY, settings);

    m_maxLayers = settings.layers;
    m_format = settings.format;
    m_width = settings.width;
    m_height = settings.height;

    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,  // mipmap level
        settings.internalFormat,
        settings.width,
        settings.height,
        settings.layers,  // depth of the texture
        0,                // border (must be 0)
        settings.format,
        settings.dataType,
        nullptr
    );
}

int TextureArray::load(const gl::ImageData& imageData, int layer) {
    if (imageData.width != m_width || imageData.height != m_height)
        throw std::runtime_error(
            std::format("Image data size does not match texture size: ({}, {})", m_width, m_height)
        );

    if (detail::toGLFormat(imageData.format) != m_format)  // todo compatible formats?
        throw std::runtime_error(
            "Image format is different from the one provided when creating this instance"
        );


    if (m_layer == -1 && layer == -1) {
        throw std::runtime_error(
            "Layer must be specified after loading with manual layer management."
        );
    }

    int targetLayer = (layer == -1) ? m_layer++ : layer;
    if (targetLayer >= m_maxLayers)
        throw std::runtime_error("Maximum layers exceeded");

    detail::SubData3D(
        GL_TEXTURE_2D_ARRAY, targetLayer, m_width, m_height, 1, imageData.format, imageData.data
    );

    printf("Layer %d: %s\n", m_layer, imageData.path.c_str());
    return targetLayer;
}

void TextureArray::load(const gl::RawImageData& rawImageData, int layer) {
    if (rawImageData.width != m_width || rawImageData.height != m_height)
        throw std::runtime_error(
            std::format("Image data size does not match texture size: ({}, {})", m_width, m_height)
        );


    if (rawImageData.format != m_format)  // todo compatible formats?
        throw std::runtime_error(
            "Image format is different from the one provided when creating this instance"
        );

    int targetLayer = (layer == -1) ? m_layer++ : layer;
    if (targetLayer >= m_maxLayers)
        throw std::runtime_error("Maximum layers exceeded");

    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,      // level of detail
        0,      // xoffset
        0,      // yoffset,
        layer,  // zoffset
        m_width,
        m_height,
        1,
        rawImageData.format,
        rawImageData.dataType,
        rawImageData.data
    );
}

int TextureArray::loadRaw(Data data, ImageFormat format, int layer) {
    int targetLayer = (layer == -1) ? m_layer++ : layer;
    if (targetLayer >= m_maxLayers)
        throw std::runtime_error("Maximum layers exceeded");

    if (detail::toGLFormat(format) != m_format)  // todo compatible formats?
        throw std::runtime_error(
            "Image format is different from the one provided when creating this instance"
        );

    detail::SubData3D(GL_TEXTURE_2D_ARRAY, targetLayer, m_width, m_height, 1, format, data);
    return targetLayer;
}