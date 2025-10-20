#include "TexturePrivate.h"


using namespace gl::detail;

void gl::detail::ConfigureTexture(GLenum type, const gl::Settings& settings) {
    glTexParameteri(type, GL_TEXTURE_WRAP_S, toGLWrap(settings.wrapS));
    glTexParameteri(type, GL_TEXTURE_WRAP_T, toGLWrap(settings.wrapT));
    glTexParameteri(type, GL_TEXTURE_WRAP_R, toGLWrap(settings.wrapR));

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, toGLFilter(settings.minFilter));
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, toGLFilter(settings.magFilter));
}

void gl::detail::Data1D(GLenum type, int width, gl::ImageFormat format, unsigned char* data) {
    glTexImage1D(
        type,
        0,                           // mipmap level
        toGLInternalFormat(format),  // internal format
        width,                       // width of the texture
        0,                           // border (must be 0)
        toGLFormat(format),          // format of the input data
        GL_UNSIGNED_BYTE,            // type of the input data
        data                         // data pointer (null = reserve space only)
    );
}

void gl::detail::Data2D(
    GLenum type,
    int width,
    int height,
    gl::ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTexImage2D(
        type,
        0,                           // mipmap level
        toGLInternalFormat(format),  // internal format
        width,                       // width of the texture
        height,                      // height of the texture
        0,                           // border (must be 0)
        toGLFormat(format),          // format of the input data
        toGLDataType(dataType),      // type of the input data
        data                         // data pointer (null = reserve space only)
    );
}

void gl::detail::Data3D(
    GLenum type, int width, int height, int depth, gl::ImageFormat format, unsigned char* data
) {
    glTexImage3D(
        type,
        0,                           // mipmap level
        toGLInternalFormat(format),  // internal format
        width,                       // width of the texture
        height,                      // height of the texture
        depth,                       // depth of the texture
        0,                           // border (must be 0)
        toGLFormat(format),          // format of the input data
        GL_UNSIGNED_BYTE,            // type of the input data
        data                         // data pointer (null = reserve space only)
    );
}

void gl::detail::SubData3D(
    GLenum type,
    int layer,
    int width,
    int height,
    int depth,
    gl::ImageFormat format,
    unsigned char* data
) {
    glTexSubImage3D(
        type,
        0,  // level of detail
        0,
        0,
        layer,  // xoffset, yoffset, zoffset
        width,
        height,
        depth,               // width, height, depth
        toGLFormat(format),  // format
        GL_UNSIGNED_BYTE,    // type
        data                 // data pointer (null = reserve space only)
    );
}