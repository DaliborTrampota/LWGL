#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace gl {

    using GLDebugCallback = void (*)(
        int source,
        int type,
        int id,
        int severity,
        int length,
        const char* message,
        const void* userParam
    );

    class Context {
      public:
        Context(Context* share = nullptr);
        virtual ~Context();

        virtual int init(int w, int h, const char* name);


        void makeCurrent();
        bool isCurrent();

        void setUserPointer(void* ptr);

        void setWindowSize(int x, int y) const;
        glm::ivec2 windowSize() const;

        void setShouldClose();
        bool shouldClose() const;

        void clearScreen(glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f)) const;
        void swapBuffers() const;

        void setKeyCallback(GLFWkeyfun callback);
        void setMouseMoveCallback(GLFWcursorposfun callback);
        void setMouseButtonCallback(GLFWmousebuttonfun callback);
        void setResizeCallback(GLFWframebuffersizefun callback);
        void setDebugCallback(GLDebugCallback callback);

        GLFWwindow* window() const { return m_window; }
        GLFWmonitor* monitor() const { return m_monitor; }

        static float time();
        static double timeDouble();

      private:
        GLFWmonitor* m_monitor = nullptr;
        GLFWwindow* m_window = nullptr;
        Context* m_sharedCtx = nullptr;

        static inline bool s_initialized = false;
        static inline int s_contextCount = 0;
    };
}  // namespace gl