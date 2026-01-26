#pragma once

#include <LWGL/Context.h>

#include <string>

namespace gl::tests {
    struct Error {
        bool hasError = false;
        std::string source;
        std::string type;
        int id;
        std::string severity;
        std::string message;

        void print() const;
    };

    class TestingContext : public Context {
      public:
        TestingContext(Context* share = nullptr);

        Error error() {
            Error error = m_lastError;
            clearError();
            return error;
        }
        void clearError() { m_lastError = Error(); }

        bool hasError() const { return m_lastError.hasError; }

        void setupErrorCapture();

      private:
        Error m_lastError;
    };
}  // namespace gl::tests