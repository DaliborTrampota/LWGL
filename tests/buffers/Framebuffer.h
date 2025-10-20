#include <LWGL/buffer/FBO.h>


void testFBO() {
    gl::FBO fbo = {gl::FBOAttachment::Color, gl::FBOAttachment::Depth};
    fbo.createTexture(
        gl::FBOAttachment::Color,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::RGBA,
            .dataType = gl::ImageDataType::UChar
        }
    );
    fbo.createTexture(
        gl::FBOAttachment::Depth,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::DEPTH,
            .dataType = gl::ImageDataType::Float
        }
    );
    fbo.bind();

    fbo.setDrawBuffers({0});  // Set Color Attachment 0 to be drawn to
    fbo.setReadBuffer(0);     // Set Color Attachment 0 to be read from

    // gl draw calls

    fbo.unbind();
}

void testFBOMultiColor() {
    gl::FBO fbo(
        {gl::FBOAttachment::Color,
         gl::FBOAttachment::Color >> 1,
         gl::FBOAttachment::Color >> 2,
         gl::FBOAttachment::Color >> 3,
         gl::FBOAttachment::DepthStencil},
        gl::FBO::Target::Draw
    );

    fbo.createTexture(
        gl::FBOAttachment::Color,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::RGBA,
            .dataType = gl::ImageDataType::UChar
        }
    );
    fbo.createTexture(
        gl::FBOAttachment::Color >> 1,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::RGBA,
            .dataType = gl::ImageDataType::UChar
        }
    );
    fbo.createTexture(
        gl::FBOAttachment::Color >> 2,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::RGBA,
            .dataType = gl::ImageDataType::UChar
        }
    );
    fbo.createTexture(
        gl::FBOAttachment::Color >> 3,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::RGBA,
            .dataType = gl::ImageDataType::UChar
        }
    );

    fbo.createTexture(
        gl::FBOAttachment::DepthStencil,
        gl::FrameBufferSettings{
            .width = 1024,
            .height = 1024,
            .format = gl::ImageFormat::DEPTH_STENCIL,
            .dataType = gl::ImageDataType::Float
        }
    );

    fbo.bind();

    fbo.setDrawBuffers({0, 1});

    // draw to color 0 and color 1 attachment

    fbo.setReadBuffer(1);
    fbo.setDrawBuffers({3});

    //read from color 1 and draw to color 3


    fbo.unbind();
}