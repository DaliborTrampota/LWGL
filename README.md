# LWGL - LightWeight Graphics Library

**LWGL** is a modern C++ wrapper library for OpenGL that provides a clean, type-safe interface for common graphics operations. It abstracts away the verbosity of raw OpenGL while maintaining performance and flexibility.

## Features

- **Buffer Management** - Type-safe VBO, EBO, VAO, and UBO abstractions
- **Texture Support** - Comprehensive texture handling (1D, 2D, Arrays, CubeMaps)
- **Shader Programs** - Simplified shader compilation and material system
- **Event System** - GLFW event integration with observer pattern
- **Window Management** - Simple window creation and lifecycle management
- **Error Handling** - Built-in OpenGL error checking with GL_GUARD
- **Type Safety** - Modern C++ templates and RAII principles
- **Zero Cost Abstraction** - Minimal overhead over raw OpenGL

## Dependencies

- **GLM** - OpenGL Mathematics library
- **GLFW** - Window and input handling
- **GLAD** - OpenGL function loader (to be dropped in future)

## Requirements

- C++17 compatible compiler
- OpenGL 4.3+ (TODO)
- CMake 3.15+

## Quick Start

### Installation

Add LWGL as a subdirectory in your CMake project:

```cmake
add_subdirectory(dependencies/lwgl)
target_link_libraries(your_target PRIVATE lwgl)
```

### Basic Usage

```cpp
#include <LWGL/Window.h>
#include <LWGL/render/Material.h>
#include <LWGL/buffer/Buffer.h>
#include <LWGL/texture/Texture2D.h>

// Create window with OpenGL context
auto api = std::make_unique<gl::GraphicsAPI>(800, 600, "My App");
gl::Window window(std::move(api));

// Create shader program (Material)
gl::Material material("shader.vert", "shader.frag", "BasicShader");

// Create vertex buffer
gl::Buffer<float> vertexBuffer(GL_STATIC_DRAW);
vertexBuffer.create(gl::VertexAttribute{0, 3, GL_FLOAT});
vertexBuffer.add(0.0f, 0.5f, 0.0f);   // Vertex 1
vertexBuffer.add(-0.5f, -0.5f, 0.0f); // Vertex 2
vertexBuffer.add(0.5f, -0.5f, 0.0f);  // Vertex 3

// Create texture
gl::Texture2D texture(0);
texture.create(gl::Settings{});
gl::ImageData imgData = gl::loadImage("texture.png");
texture.load(imgData);

// Render loop
while (!window.shouldClose()) {
    window.beginFrame();
    
    material.use();
    texture.bind();
    vertexBuffer.bind();
    
    glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.length());
    
    window.endFrame();
}
```

## Core Components

### Window Management

The `Window` class manages the application window and OpenGL context:

```cpp
#include <LWGL/Window.h>
#include <LWGL/gl/GraphicsAPI.h>

// Create window
auto api = std::make_unique<gl::GraphicsAPI>(1920, 1080, "Window Title");
gl::Window window(std::move(api));

// Window operations
window.mouseLock(true);           // Lock cursor
window.setRenderFlags();          // Set default render flags
float deltaTime = window.time();  // Get elapsed time
window.close();                   // Request window close

// Frame lifecycle
window.beginFrame();  // Clear and prepare
// ... rendering ...
window.endFrame();    // Swap buffers
```

### Buffers

#### Vertex Buffer Object (VBO)

```cpp
#include <LWGL/buffer/Buffer.h>

// Create typed buffer
gl::Buffer<glm::vec3> positions(GL_STATIC_DRAW);
positions.create(gl::VertexAttribute{
    .location = 0,
    .size = 3,
    .type = GL_FLOAT
});

// Add vertex data
positions.add(glm::vec3(0.0f, 1.0f, 0.0f));
positions.add(glm::vec3(-1.0f, -1.0f, 0.0f));
positions.add(glm::vec3(1.0f, -1.0f, 0.0f));

// Use buffer
positions.bind();
glDrawArrays(GL_TRIANGLES, 0, positions.length());
```

#### Element Buffer Object (EBO)

```cpp
std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
vertexBuffer.setElements(indices);

vertexBuffer.bind();
glDrawElements(GL_TRIANGLES, vertexBuffer.elements(), GL_UNSIGNED_INT, 0);
```

#### Uniform Buffer Object (UBO)

```cpp
#include <LWGL/buffer/UBO.h>

// Create UBO with explicit layout
gl::UBO cameraUBO(0, {
    gl::Type::Mat4,  // projection
    gl::Type::Mat4,  // view
    gl::Type::Vec3   // cameraPos
}, "CameraBlock");

cameraUBO.create();
cameraUBO.bindToProgram(material.programID());

// Update data
cameraUBO.setSubData(0, &projectionMatrix);
cameraUBO.setSubData(1, &viewMatrix);
```

### Textures

#### 2D Textures

```cpp
#include <LWGL/texture/Texture2D.h>

// Create and load texture
gl::Texture2D texture(0);  // Texture unit 0
texture.create(gl::Settings::LinearClampToEdge());

// Load from image
gl::ImageData img = gl::loadImage("texture.png");
texture.load(img);

// Or load raw data
texture.loadRaw(width, height, channels, gl::ImageFormat::RGBA, data);

// Bind for rendering
texture.bind();
```

#### Texture Arrays

```cpp
#include <LWGL/texture/TextureArray.h>

gl::TextureArray textureArray(0);
textureArray.create(gl::ArraySettings{
    .layers = 16,
    .width = 256,
    .height = 256,
    .format = gl::ImageFormat::RGBA
});

// Load individual layers
for (int i = 0; i < 16; i++) {
    gl::ImageData layer = gl::loadImage("layer_" + std::to_string(i) + ".png");
    textureArray.setLayer(i, layer);
}
```

#### CubeMaps

```cpp
#include <LWGL/texture/CubeMap.h>

gl::CubeMap skybox(0);
skybox.create(gl::Settings::Cubemap());

// Load six faces
skybox.loadFace(gl::CubeFace::Right,  rightImage);
skybox.loadFace(gl::CubeFace::Left,   leftImage);
skybox.loadFace(gl::CubeFace::Top,    topImage);
skybox.loadFace(gl::CubeFace::Bottom, bottomImage);
skybox.loadFace(gl::CubeFace::Front,  frontImage);
skybox.loadFace(gl::CubeFace::Back,   backImage);
```

### Materials (Shader Programs)

```cpp
#include <LWGL/render/Material.h>

// Create material from shader files
gl::Material material("shader.vert", "shader.frag", "MyMaterial");

// With geometry shader
gl::Material advMaterial(
    "shader.vert",
    "shader.geom",
    "shader.frag",
    "AdvancedMaterial"
);

// Use material
material.use();

// Set uniforms
material.setBool("useTexture", true);
material.setInt("textureSampler", 0);
material.setFloat("opacity", 0.8f);
material.setMat4("model", modelMatrix);

// Bind UBO
material.bindUBO(cameraUBO);
```

### Event System

LWGL provides a type-safe event system integrated with GLFW:

```cpp
#include <LWGL/events/GLEvents.h>

// Window implements GLEventSite
class MyApp : public gl::Window {
public:
    MyApp(std::unique_ptr<gl::GraphicsAPI> api) 
        : Window(std::move(api)) {}
    
    void windowResizeEvent(ResizeEvent* event) override {
        std::cout << "Window resized: " 
                  << event->width << "x" << event->height << std::endl;
    }
    
    void mouseEvent(MouseEvent* event) override {
        std::cout << "Mouse: " << event->x << ", " << event->y << std::endl;
    }
    
    void keyboardEvent(KeyboardEvent* event) override {
        if (event->key == GLFW_KEY_ESCAPE && event->action == GLFW_PRESS) {
            close();
        }
    }
};
```

## Advanced Features

### Vertex Layout System

For complex vertex structures with multiple attributes:

```cpp
#include <LWGL/buffer/VertexLayout.h>

// Define vertex structure
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

// Create layout
gl::VertexLayout layout;
layout.addAttribute({0, 3, GL_FLOAT, sizeof(Vertex), 0});                          // position
layout.addAttribute({1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal)});   // normal
layout.addAttribute({2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord)}); // texCoord
```

### Error Handling

LWGL includes built-in error checking:

```cpp
// GL_GUARD macro checks for OpenGL errors in debug builds and whether OpenGL code was called from the context window thread
GL_GUARD  // Checks and reports any pending OpenGL errors

// Errors are automatically checked after OpenGL calls in LWGL functions
```

### Texture Settings Presets

```cpp
// Pixelated textures (for pixel art)
auto pixelSettings = gl::Settings::Pixelated();

// Cubemap textures
auto cubeSettings = gl::Settings::Cubemap();

// Custom settings
gl::Settings custom{
    .wrapS = gl::Settings::ClampToEdge,
    .wrapT = gl::Settings::MirroredRepeat,
    .minFilter = gl::Settings::Nearest,
    .magFilter = gl::Settings::Linear
};
```

## Implementation Status

- **Buffers**
  - [x] VBO (Vertex Buffer Object)
  - [x] EBO (Element Buffer Object)
  - [x] VAO (Vertex Array Object)
  - [x] UBO (Uniform Buffer Object)
  - [ ] SSBO (Shader Storage Buffer Object) - Planned

- **Textures**
  - [x] 1D Textures
  - [x] 2D Textures
  - [x] 2D Texture Arrays
  - [x] Cubemaps
  - [ ] 1D Texture Arrays - Planned

- **Rendering**
  - [x] Shader compilation
  - [x] Program linking
  - [x] Material system
  - [ ] Frame Buffer Objects (FBO) - Planned

- **Other**
  - [x] Window management
  - [x] Event system
  - [x] Error handling
  - [x] RAII resource management

## Architecture

LWGL follows modern C++ principles:

- **RAII** - Resources are automatically managed (no manual cleanup needed)
- **Type Safety** - Template-based buffers prevent type mismatches
- **Move Semantics** - Efficient resource transfer with move constructors
- **Zero-Copy** - Minimal data copying in hot paths
- **Header-Only Where Possible** - Many components are header-only templates

### Design Patterns

- **Facade Pattern** - Simplified interface over complex OpenGL API
- **Observer Pattern** - Event system for input and window events
- **Strategy Pattern** - Pluggable texture settings and configurations

## Performance

LWGL is designed for:
- **Zero overhead** abstraction where possible
- **Deferred updates** with dirty flags
- **Batch operations** for buffer updates
- **Minimal state changes** through smart binding

## Roadmap

- [ ] Complete SSBO implementation
- [ ] Add 1D texture array support
- [ ] Implement Frame Buffer Objects (FBO)
- [ ] Add compute shader support
- [ ] Multi-threaded resource loading
- [ ] Learn Vulkan

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

Copyright 2025 LWGL Contributors

## Example Projects

LWGL is currently used in:
- **VoxelEngine** - A voxel-based game engine


And check out also my **SEUIL** - Simple Enough UI Library (as well for OpenGL)

---

**Lightweight, Modern, Efficient**