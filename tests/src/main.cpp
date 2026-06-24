#include "TestingContext.h"

// Force dedicated GPU (win only) (fixed bindless textures [probably laptop only issue?])
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;  // for nvidia
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;    // for amd
}


#include "tests/Textures.h"


// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
int main(int argc, char* argv[]) {
    gl::tests::TestingContext context;

    context.init(800, 600, "LWGL Test");

    context.setUserPointer(&context);
    context.setupErrorCapture();


    testTexture2DImmutable(context);
    testTexture2DMutable(context);

    testCubeMapImmutable(context);
    testCubeMapMutable(context);
    return 0;
}