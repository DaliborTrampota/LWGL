#include "LWGL/GLTypes.h"
#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/TextureBase.h"


namespace gl::detail {

    void ConfigureTexture(GLuint texID, const TextureParams& params);
    void Data1D(
        GLenum type,
        int width,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );

    void Data1DImmutable(GLuint texID, int width, ImageFormat format);

    void SubData1D(
        GLuint texID,
        int x,
        int width,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );

    void Data2D(
        GLenum type,
        int width,
        int height,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );

    void Data2DImmutable(GLuint texID, int width, int height, ImageFormat format);

    void SubData2D(
        GLuint texID,
        int x,
        int y,
        int width,
        int height,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );

    void Data3D(
        GLenum type,
        int width,
        int height,
        int depth,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );

    void Data3DImmutable(GLuint texID, int width, int height, int depth, ImageFormat format);

    void SubData3D(
        GLuint texID,
        int x,
        int y,
        int zOrLayer,
        int width,
        int height,
        int depth,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );
}  // namespace gl::detail
