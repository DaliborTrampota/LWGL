
#include <LWGL/texture/CubeMap.h>
#include <LWGL/texture/Texture2D.h>


#include "../ErrorMessages.h"
#include "../TestingContext.h"
#include "../Utils.h"


using namespace gl;
using namespace gl::tests;


namespace {
    constexpr GLenum toGLCubeFace(CubeFace face) {
        switch (face) {
            case CubeFace::Right: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case CubeFace::Left: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case CubeFace::Top: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case CubeFace::Bottom: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case CubeFace::Back: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
            case CubeFace::Front: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        }
    }
}  // namespace


inline void testTexture2DImmutable(TestingContext& context) {
    Texture2D texture{true};
    texture.create(TextureParams(TextureParams::Wrap::ClampToEdge, TextureParams::Filter::Linear));
    texture.allocate(
        TextureStorage{
            .width = 100,
            .height = 100,
        }
    );

    Data data = generateData(100, 100);
    texture.upload(ImageFormat::RGBA, data);

    texture.allocate({
        .width = 200,
        .height = 200,
        .format = ImageFormat::RGBA,
        .dataType = ImageDataType::UChar,
    });
    Error error = context.error();
    ASSERT(error.message == TEXTURE_IMMUTABLE);

    texture.activate(0);
    texture.unbind(0);

    ASSERT(!context.hasError());
}

inline void testTexture2DMutable(TestingContext& context) {
    Texture2D texture{false};
    texture.create(TextureParams(TextureParams::Wrap::ClampToEdge, TextureParams::Filter::Linear));
    texture.allocate(
        TextureStorage{
            .width = 100,
            .height = 100,
        }
    );

    Data data = generateData(100, 100);
    texture.upload(ImageFormat::RGBA, data);
    texture.activate(0);
    ASSERT(!context.hasError());
    delete[] data;

    texture.allocate({
        .width = 200,
        .height = 200,
        .format = ImageFormat::RGBA,
        .dataType = ImageDataType::UChar,
    });
    data = generateData(200, 200);
    texture.upload(ImageFormat::RGBA, data);
    texture.activate(0);
    ASSERT(!context.hasError());
    delete[] data;
}

inline void testCubeMapImmutable(TestingContext& context) {
    CubeMap cubeMap{true};
    cubeMap.create(TextureParams(TextureParams::Wrap::ClampToEdge, TextureParams::Filter::Linear));

    cubeMap.allocate(
        TextureStorage{
            .width = 100,
            .height = 100,
        }
    );

    Data data = generateData(100, 100);
    for (int i = 0; i < 6; i++) {
        cubeMap.upload(static_cast<CubeFace>(i), 100, ImageFormat::RGBA, data);
    }

    cubeMap.allocate({
        .width = 200,
        .height = 200,
        .format = ImageFormat::RGBA,
        .dataType = ImageDataType::UChar,
    });
    Error error = context.error();
    ASSERT(error.message == TEXTURE_IMMUTABLE);

    for (int i = 0; i < 6; i++) {
        int faceDataSize = 100 * 100 * 4;
        Data faceData = new unsigned char[faceDataSize];
        glGetTextureSubImage(
            cubeMap.id(), 0, 0, 0, i, 100, 100, 1, GL_RGBA, GL_UNSIGNED_BYTE, faceDataSize, faceData
        );
        ASSERT(!context.hasError());
        ASSERT(dataEquals(faceData, data, 100, 100, 4));
        delete[] faceData;
    }

    cubeMap.activate(0);
    ASSERT(!context.hasError());
}

inline void testCubeMapMutable(TestingContext& context) {
    CubeMap cubeMap{false};
    cubeMap.create(TextureParams(TextureParams::Wrap::ClampToEdge, TextureParams::Filter::Linear));

    Data data = generateData(100, 100);
    cubeMap.allocate(
        TextureStorage{
            .width = 100,
            .height = 100,
        }
    );

    for (int i = 0; i < 6; i++) {
        cubeMap.upload(static_cast<CubeFace>(i), 100, ImageFormat::RGBA, data);
    }
    ASSERT(!context.hasError());

    for (int i = 0; i < 6; i++) {
        int faceDataSize = 100 * 100 * 4;
        Data faceData = new unsigned char[faceDataSize];
        glGetTexImage(
            toGLCubeFace(static_cast<CubeFace>(i)), 0, GL_RGBA, GL_UNSIGNED_BYTE, faceData
        );
        ASSERT(!context.hasError());
        ASSERT(dataEquals(faceData, data, 100, 100, 4));
        delete[] faceData;
    }
    delete[] data;
}
