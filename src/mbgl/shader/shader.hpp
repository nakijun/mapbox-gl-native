#ifndef MBGL_RENDERER_SHADER
#define MBGL_RENDERER_SHADER

#include <mbgl/gl/gl.hpp>
#include <mbgl/gl/gl_handle.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace mbgl {

class Shader : private util::noncopyable {
public:
    Shader(const GLchar *name, const GLchar *vertex, const GLchar *fragment);

    ~Shader();
    const GLchar *name;

    inline GLuint getID() const {
        return **program;
    }

    virtual void bind(GLbyte *offset) = 0;

protected:
    GLint a_pos = -1;

private:
    bool compileShader(gl::ShaderPtr& shader, GLenum type, const GLchar *source[]);

    gl::ProgramPtr program;
    gl::ShaderPtr vertexShader;
    gl::ShaderPtr fragmentShader;
};

} // namespace mbgl

#endif
