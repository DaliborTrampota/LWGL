#include "LWGL/Context.h"


#include <glad/glad.h>
#include <iostream>

#include "GLFW/glfw3.h"


std::thread::id glContextID;

using namespace gl;

Context::Context(Context* share) : m_sharedCtx(share) {
    if (!s_initialized) {
        glfwInit();
        s_initialized = true;
    }
}

Context::~Context() {
    if (m_window)
        glfwDestroyWindow(m_window);

    s_contextCount--;
    if (s_contextCount == 0) {
        glfwTerminate();
        s_initialized = false;
    }
}


int Context::init(int w, int h, const char* name) {
    INIT_GL_THREAD;  //TODO keep?

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_monitor = glfwGetPrimaryMonitor();
    m_window = glfwCreateWindow(w, h, name, nullptr, m_sharedCtx ? m_sharedCtx->m_window : nullptr);

    if (!m_window) {
        printf("Failed to create GLFW window");
        return 0;
    }

    makeCurrent();
    glfwSwapInterval(0);  // disable vsync

    // int mw, mh;
    // glfwGetMonitorWorkarea(m_monitor, nullptr, nullptr, &mw, &mh);

    // w *= 0.6;
    // h *= 0.6;


    if (!m_sharedCtx) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            printf("Failed to initialize GLAD");
            return 0;
        }
    }

    glfwSetWindowPos(m_window, 500, 200);

    // GLint max_layers, max_units;
    // glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
    // glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_units);
    // printf("Max layers: %d\nMax units: %d\n", max_layers, max_units);

    // if (GLAD_GL_ARB_bindless_texture)
    //     printf("Bindless textures supported!\n");
    // else
    //     printf("Bindless textures NOT supported!\n");

    s_contextCount++;
    return 1;
}

void Context::makeCurrent() {
    glfwMakeContextCurrent(m_window);
}

bool Context::isCurrent() {
    return glfwGetCurrentContext() == m_window;
}


void Context::setWindowSize(int x, int y) const {
    glfwSetWindowSize(m_window, x, y);
}

glm::ivec2 Context::windowSize() const {
    glm::ivec2 size;
    glfwGetWindowSize(m_window, &size.x, &size.y);
    return size;
}

void Context::setShouldClose() {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Context::shouldClose() const {
    return glfwWindowShouldClose(m_window) == GL_TRUE;
}

void Context::clearScreen(glm::vec4 color) const {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Context::swapBuffers() const {
    glfwSwapBuffers(m_window);
}

void Context::setUserPointer(void* ptr) {
    glfwSetWindowUserPointer(m_window, ptr);
}

void Context::setKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(m_window, callback);
}

void Context::setMouseMoveCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(m_window, callback);
}

void Context::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(m_window, callback);
}

void Context::setResizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(m_window, callback);
}

void Context::setDebugCallback(GLDebugCallback callback) {
    // if (!m_window)
    //     throw std::runtime_error("Calling registerCallbacks before calling init!");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)callback, nullptr);
}

float Context::time() {
    return glfwGetTime();
}