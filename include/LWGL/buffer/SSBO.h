#pragma once

#include <stdexcept>
#include <utility>
#include <vector>


#include <glad/glad.h>

namespace gl {

    template <typename T>
    class SSBO {
      public:
        SSBO(GLenum drawMode = GL_STATIC_DRAW) : m_drawMode(drawMode) {};
        ~SSBO() {
            if (m_id != 0)
                glDeleteBuffers(1, &m_id);
        }

        SSBO(const SSBO&) = delete;
        SSBO& operator=(const SSBO&) = delete;

        SSBO(SSBO&& other) noexcept
            : m_id(other.m_id),
              m_data(std::move(other.m_data)),
              m_dirty(other.m_dirty),
              m_drawMode(other.m_drawMode),
              m_gpuCapacity(other.m_gpuCapacity) {
            other.m_id = 0;
            other.m_gpuCapacity = 0;
        }

        SSBO& operator=(SSBO&& other) noexcept {
            if (this != &other) {
                if (m_id != 0)
                    glDeleteBuffers(1, &m_id);
                m_id = std::exchange(other.m_id, 0);
                m_data = std::move(other.m_data);
                m_dirty = other.m_dirty;
                m_drawMode = other.m_drawMode;
                m_gpuCapacity = std::exchange(other.m_gpuCapacity, 0);
            }
            return *this;
        }

        void create(size_t capacity) {
            if (m_id != 0)
                throw std::runtime_error("SSBO already created");
            glCreateBuffers(1, &m_id);
            glNamedBufferData(m_id, capacity * sizeof(T), nullptr, m_drawMode);
            m_gpuCapacity = capacity;
        }

        void upload() {
            if (m_id == 0)
                throw std::runtime_error("SSBO not created");
            if (!m_dirty)
                return;

            if (m_gpuCapacity < m_data.size())
                throw std::runtime_error(
                    "SSBO upload exceeds GPU capacity; call allocate() before upload."
                );
            glNamedBufferSubData(
                m_id, 0, m_data.size() * sizeof(T), m_data.empty() ? nullptr : m_data.data()
            );

            m_dirty = false;
        }

        void bind(unsigned bindingPoint) const {
            if (m_id == 0)
                throw std::runtime_error("SSBO not created");
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_id);
        }

        void unbind(unsigned bindingPoint) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, 0);
        }

        void clear() {
            m_data.clear();
            m_dirty = true;
        }

        /// @brief Reserves space for data on CPU
        void reserve(size_t size) { m_data.reserve(size); }

        /// @brief Grows GPU-side buffer to requested capacity
        void allocate(size_t capacity) {
            if (!m_id)
                throw std::runtime_error("SSBO not created");
            if (capacity < m_data.size())
                throw std::runtime_error("SSBO allocate() cannot shrink below current data size");

            glNamedBufferData(m_id, capacity * sizeof(T), nullptr, m_drawMode);
            m_gpuCapacity = capacity;
            m_dirty = true;
        }

        void setData(std::vector<T>&& data) {
            m_data = std::move(data);
            m_dirty = true;
        }

        void add(const T& item) {
            m_data.push_back(item);
            m_dirty = true;
        }

        const std::vector<T>& data() const { return m_data; }
        std::vector<T>& data() { return m_data; }

        size_t capacity() const { return m_gpuCapacity; }
        size_t remainingCapacity() const { return m_gpuCapacity - m_data.size(); }

      private:
        unsigned int m_id = 0;
        GLenum m_drawMode;
        size_t m_gpuCapacity = 0;

        std::vector<T> m_data;
        bool m_dirty = false;
    };
}  // namespace gl
