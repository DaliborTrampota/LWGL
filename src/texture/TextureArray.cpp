#include "LWGL/texture/TextureArray.h"
#include "../detail/TexturePrivate.h"
#include "../detail/conversions.h"
#include "LWGL/texture/ImageData.h"


#include <format>
#include <stdexcept>

#include <glad/glad.h>
#include <tools/stb_image.h>

using namespace gl;


void TextureArray::create(TextureParams params) {
    if (m_id != 0)
        throw std::runtime_error("TextureArray already created");

    if (m_immutable) {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_id);
        detail::ConfigureTexture(m_id, params);
    } else {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
        detail::ConfigureTexture(m_id, params);
    }
}

void TextureArray::allocate(TextureStorage storage) {
    if (m_id == 0)
        throw std::runtime_error("TextureArray not created");

    m_width = storage.width;
    m_height = storage.height;
    m_maxLayers = storage.depth;
    // m_channels // TODO

    if (m_immutable) {
        detail::Data3DImmutable(m_id, m_width, m_height, m_maxLayers, storage.format);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
        detail::Data3D(
            GL_TEXTURE_2D_ARRAY,
            m_width,
            m_height,
            m_maxLayers,
            storage.format,
            nullptr,
            storage.dataType
        );
    }
}

int TextureArray::upload(const gl::ImageData& imageData, int layer) {
    if (m_id == 0)
        throw std::runtime_error("TextureArray not created");

    int targetLayer = compabilityCheck(imageData.width, imageData.height, layer);

    detail::SubData3D(
        m_id,
        0,
        0,
        targetLayer,
        imageData.width,
        imageData.height,
        1,
        imageData.format,
        imageData.data
    );

    printf("Layer %d: %s\n", m_layer, imageData.path.c_str());
    return targetLayer;
}

int TextureArray::upload(const gl::RawImageData& rawImageData, int layer) {
    if (m_id == 0)
        throw std::runtime_error("TextureArray not created");

    int targetLayer = compabilityCheck(rawImageData.width, rawImageData.height, layer);

    glTextureSubImage3D(
        m_id,
        0,            // level of detail
        0,            // xoffset
        0,            // yoffset,
        targetLayer,  // zoffset
        rawImageData.width,
        rawImageData.height,
        1,
        rawImageData.format,
        rawImageData.dataType,
        rawImageData.data
    );
    return targetLayer;
}

int TextureArray::upload(ImageFormat format, Data data, int layer) {
    if (m_id == 0)
        throw std::runtime_error("TextureArray not created");

    int targetLayer = compabilityCheck(m_width, m_width, layer);

    detail::SubData3D(m_id, 0, 0, targetLayer, m_width, m_height, 1, format, data);
    return targetLayer;
}

int TextureArray::compabilityCheck(int w, int h, int layer) {
    if (w > m_width || h > m_height)
        throw std::runtime_error(
            std::format("Image data size is larger than texture size: ({}, {})", m_width, m_height)
        );

    if (layer != -1) {
        m_layer = -1;  // disable automatic layer increment
    } else {
        if (m_layer == -1) {
            throw std::runtime_error(
                "Layer must be specified after loading with manual layer management."
            );
        }
    }


    int targetLayer = (layer == -1) ? m_layer++ : layer;
    if (targetLayer >= m_maxLayers)
        throw std::runtime_error("Maximum layers exceeded");

    return targetLayer;
}