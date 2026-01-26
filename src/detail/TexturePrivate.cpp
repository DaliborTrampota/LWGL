#include "TexturePrivate.h"
#include "conversions.h"

using namespace gl::detail;

void gl::detail::ConfigureTexture(GLuint texID, const gl::TextureParams& params) {
    glTextureParameteri(texID, GL_TEXTURE_WRAP_S, toGLWrap(params.wrapS));
    glTextureParameteri(texID, GL_TEXTURE_WRAP_T, toGLWrap(params.wrapT));
    glTextureParameteri(texID, GL_TEXTURE_WRAP_R, toGLWrap(params.wrapR));

    glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, toGLFilter(params.minFilter));
    glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, toGLFilter(params.magFilter));

    glTextureParameterfv(texID, GL_TEXTURE_BORDER_COLOR, params.borderColor);
}

void gl::detail::Data1D(
    GLenum type, int width, ImageFormat format, unsigned char* data, gl::ImageDataType dataType
) {
    glTexImage1D(
        type,
        0,  // mipmap level index
        toGLInternalFormat(format),
        width,
        0,  // border (must be 0)
        toGLFormat(format),
        toGLDataType(dataType),  // TODO dataType with internal format
        data
    );
}

void gl::detail::Data1DImmutable(GLuint texID, int width, ImageFormat format) {
    glTextureStorage1D(
        texID,
        1,  // mipmap level count
        toGLInternalFormat(format),
        width
    );
}

void gl::detail::SubData1D(
    GLuint texID,
    int x,
    int width,
    ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTextureSubImage1D(texID, 0, x, width, toGLFormat(format), toGLDataType(dataType), data);
}

void gl::detail::Data2D(
    GLenum type,
    int width,
    int height,
    ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTexImage2D(
        type,
        0,  // mipmap level index
        toGLInternalFormat(format, dataType),
        width,
        height,
        0,  // border (must be 0)
        toGLFormat(format),
        toGLDataType(dataType),
        data
    );
}

void gl::detail::Data2DImmutable(GLuint texID, int width, int height, ImageFormat format) {
    glTextureStorage2D(
        texID,
        1,  // mipmap level count
        toGLInternalFormat(format),
        width,
        height
    );
}

void gl::detail::SubData2D(
    GLuint texID,
    int x,
    int y,
    int width,
    int height,
    ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTextureSubImage2D(
        texID, 0, x, y, width, height, toGLFormat(format), toGLDataType(dataType), data
    );
}

void gl::detail::Data3D(
    GLenum type,
    int width,
    int height,
    int depth,
    ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTexImage3D(
        type,
        0,  // mipmap level
        toGLInternalFormat(format),
        width,
        height,
        depth,  // depth of the texture
        0,      // border (must be 0)
        toGLFormat(format),
        toGLDataType(dataType),  // TODO dataType with internal format
        data
    );
}

void gl::detail::Data3DImmutable(
    GLuint texID, int width, int height, int depth, ImageFormat format
) {
    glTextureStorage3D(
        texID,
        1,  // mipmap level
        toGLInternalFormat(format),
        width,
        height,
        depth
    );
}

void gl::detail::SubData3D(
    GLuint texID,
    int x,
    int y,
    int zOrLayer,
    int width,
    int height,
    int depth,
    ImageFormat format,
    unsigned char* data,
    gl::ImageDataType dataType
) {
    glTextureSubImage3D(
        texID,
        0,         // mipmap level index
        x,         // xoffset
        y,         // yoffset,
        zOrLayer,  // zoffset
        width,
        height,
        depth,
        toGLFormat(format),
        toGLDataType(dataType),  // TODO dataType with internal format
        data
    );
}