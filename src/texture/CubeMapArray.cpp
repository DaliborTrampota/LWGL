#include "LWGL/texture/CubeMapArray.h"
#include "../detail/TexturePrivate.h"


#include <glad/glad.h>
#include <stdexcept>


using namespace gl;

void CubeMapArray::create(TextureParams params) {
    if (m_id != 0)
        throw std::runtime_error("CubeMapArray already created");

    if (m_immutable) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &m_id);
    } else {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_id);
    }
    detail::ConfigureTexture(m_id, params);
}

void CubeMapArray::allocate(TextureStorage storage) {
    if (m_id == 0)
        throw std::runtime_error("CubeMapArray not created");
    if (storage.width <= 0)
        throw std::runtime_error("CubeMapArray::allocate: width must be greater than zero");
    if (storage.height != 0 && storage.height != storage.width)
        throw std::runtime_error(
            "CubeMapArray::allocate: cubemap faces must be square (height must equal width)"
        );
    if (storage.depth <= 0)
        throw std::runtime_error(
            "CubeMapArray::allocate: depth (layers) must be greater than zero"
        );
    m_width = storage.width;
    m_layers = storage.depth;
    // m_channels = storage.channels; // TODO deduce from format?

    if (m_immutable) {
        detail::Data3DImmutable(
            m_id, m_width, m_width, layerFaces(), storage.format, storage.dataType
        );
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_id);
        detail::Data3D(
            GL_TEXTURE_CUBE_MAP_ARRAY,
            m_width,
            m_width,
            layerFaces(),
            storage.format,
            nullptr,
            storage.dataType
        );
    }
}

void CubeMapArray::upload(int layer, CubeFace face, int width, ImageFormat format, Data data) {
    if (m_id == 0)
        throw std::runtime_error("CubeMapArray not created");

    if (width <= 0)
        throw std::runtime_error("Width must be greater than zero");

    if (width != m_width)
        throw std::runtime_error(
            "CubeMapArray::upload: width must be equal to the allocated width"
        );

    if (layer < 0 || layer >= m_layers)
        throw std::runtime_error(
            "CubeMapArray::upload: layer must be between 0 and the number of layers"
        );

    if (!data)
        throw std::runtime_error("CubeMapArray::upload: data is null");

    detail::SubData3D(
        m_id, 0, 0, layer * 6 + static_cast<int>(face), width, width, 1, format, data
    );
}