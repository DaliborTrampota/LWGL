#include "TestingContext.h"

#include <glad/glad.h>

#include <iostream>

namespace {
    const char* getDebugSource(GLenum source) {
        switch (source) {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
            case GL_DEBUG_SOURCE_APPLICATION: return "Application";
            case GL_DEBUG_SOURCE_OTHER: return "Other";
            default: return "Unknown";
        }
    }

    const char* getDebugType(GLenum type) {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
            case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
            case GL_DEBUG_TYPE_MARKER: return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
            case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
            case GL_DEBUG_TYPE_OTHER: return "Other";
            default: return "Unknown";
        }
    }

    const char* getDebugSeverity(GLenum severity) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: return "High";
            case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
            case GL_DEBUG_SEVERITY_LOW: return "Low";
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
            default: return "Unknown";
        }
    }
}  // namespace

using namespace gl::tests;

TestingContext::TestingContext(Context* share) : Context(share) {}

void TestingContext::setupErrorCapture() {
    setDebugCallback([](int source,
                        int type,
                        int id,
                        int severity,
                        int length,
                        const char* message,
                        const void* userParam) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            return;

        TestingContext* self = (TestingContext*)(userParam);
        if (self->m_lastError.hasError)
            throw std::runtime_error("Multiple unchecked errors");

        self->m_lastError = Error{
            .hasError = true,
            .source = getDebugSource(source),
            .type = getDebugType(type),
            .id = id,
            .severity = getDebugSeverity(severity),
            .message = message,
        };
    });
}

void Error::print() const {
    std::cout << "Error: " << source << " " << type << " " << id << " " << severity << " "
              << message << std::endl;
}