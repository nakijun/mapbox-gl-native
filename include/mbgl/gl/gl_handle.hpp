#ifndef MBGL_GL_GL_HANDLE
#define MBGL_GL_GL_HANDLE

#include <mbgl/gl/gl.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <memory>

namespace mbgl {
namespace gl {

struct Handle : public util::noncopyable {
    virtual ~Handle() = default;

    operator bool() const { return handle; }
    GLuint const &operator*() const { return handle; }

protected:
    Handle() = default;
    Handle(GLuint handle_) : handle(handle_) {}

    GLuint handle = 0;
};

struct ProgramHandle : public Handle {
    ProgramHandle() : Handle(MBGL_CHECK_ERROR(glCreateProgram())) {}
    ~ProgramHandle() { MBGL_CHECK_ERROR(glDeleteProgram(handle)); }
};

struct ShaderHandle : public Handle {
    ShaderHandle(GLenum type) : Handle(MBGL_CHECK_ERROR(glCreateShader(type))) {}
    ~ShaderHandle() { MBGL_CHECK_ERROR(glDeleteShader(handle)); }
};

struct BufferHandle : public Handle {
    BufferHandle() { MBGL_CHECK_ERROR(glGenBuffers(1, &handle)); }
    ~BufferHandle() { MBGL_CHECK_ERROR(glDeleteBuffers(1, &handle)); }
};

using ProgramPtr = std::unique_ptr<ProgramHandle>;
using ShaderPtr = std::unique_ptr<ShaderHandle>;
using BufferPtr = std::unique_ptr<BufferHandle>;

} // namespace gl
} // namespace mbgl

#endif // MBGL_GL_GL_HANDLE
