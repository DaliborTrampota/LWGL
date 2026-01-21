#include "LWGL/GLTypes.h"
#include "LWGL/texture/ImageData.h"
#include "LWGL/texture/TextureBase.h"


namespace gl::detail {

    void ConfigureTexture(GLenum type, const Settings& settings);
    void Data1D(
        GLenum type,
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
    void SubData2D(
        GLenum type,
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
    void SubData3D(
        GLenum type,
        int layer,
        int width,
        int height,
        int depth,
        ImageFormat format,
        unsigned char* data,
        gl::ImageDataType dataType = gl::ImageDataType::UChar
    );
}  // namespace gl::detail
