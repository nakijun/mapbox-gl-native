#include <mbgl/shader/shader.hpp>
#include <mbgl/gl/gl.hpp>
#include <mbgl/util/stopwatch.hpp>
#include <mbgl/util/exception.hpp>
#include <mbgl/platform/log.hpp>
#include <mbgl/platform/platform.hpp>

#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace mbgl;

Shader::Shader(const char *name_, const GLchar *vertSource, const GLchar *fragSource)
    : name(name_)
    , program(std::make_unique<gl::ProgramHandle>())
{
    util::stopwatch stopwatch("shader compilation", Event::Shader);

    if (!compileShader(vertexShader, GL_VERTEX_SHADER, &vertSource)) {
        Log::Error(Event::Shader, "Vertex shader %s failed to compile: %s", name, vertSource);
        program.reset();
        throw util::ShaderException(std::string { "Vertex shader " } + name + " failed to compile");
    }

    if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER, &fragSource)) {
        Log::Error(Event::Shader, "Fragment shader %s failed to compile: %s", name, fragSource);
        vertexShader.reset();
        program.reset();
        throw util::ShaderException(std::string { "Fragment shader " } + name + " failed to compile");
    }

    // Attach shaders
    MBGL_CHECK_ERROR(glAttachShader(**program, **vertexShader));
    MBGL_CHECK_ERROR(glAttachShader(**program, **fragmentShader));

    {
        // Link program
        GLint status;
        MBGL_CHECK_ERROR(glLinkProgram(**program));

        MBGL_CHECK_ERROR(glGetProgramiv(**program, GL_LINK_STATUS, &status));
        if (status == 0) {
            GLint logLength;
            MBGL_CHECK_ERROR(glGetProgramiv(**program, GL_INFO_LOG_LENGTH, &logLength));
            const auto log = std::make_unique<GLchar[]>(logLength);
            if (logLength > 0) {
                MBGL_CHECK_ERROR(glGetProgramInfoLog(**program, logLength, &logLength, log.get()));
                Log::Error(Event::Shader, "Program failed to link: %s", log.get());
            }

            vertexShader.reset();
            fragmentShader.reset();
            program.reset();
            throw util::ShaderException(std::string { "Program " } + name + " failed to link: " + log.get());
        }
    }

    a_pos = MBGL_CHECK_ERROR(glGetAttribLocation(**program, "a_pos"));
}


bool Shader::compileShader(gl::ShaderPtr& shader, GLenum type, const GLchar *source[]) {
    GLint status;

    shader = std::make_unique<gl::ShaderHandle>(type);
    const GLsizei lengths = static_cast<GLsizei>(std::strlen(*source));
    MBGL_CHECK_ERROR(glShaderSource(**shader, 1, source, &lengths));

    MBGL_CHECK_ERROR(glCompileShader(**shader));

    MBGL_CHECK_ERROR(glGetShaderiv(**shader, GL_COMPILE_STATUS, &status));
    if (status == 0) {
        GLint logLength;
        MBGL_CHECK_ERROR(glGetShaderiv(**shader, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength > 0) {
            const auto log = std::make_unique<GLchar[]>(logLength);
            MBGL_CHECK_ERROR(glGetShaderInfoLog(**shader, logLength, &logLength, log.get()));
            Log::Error(Event::Shader, "Shader failed to compile: %s", log.get());
        }

        shader.reset();
        return false;
    }

    MBGL_CHECK_ERROR(glGetShaderiv(**shader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        Log::Error(Event::Shader, "Shader %s failed to compile.", name);
        shader.reset();
        return false;
    }

    return true;
}

Shader::~Shader() {
    if (program) {
        MBGL_CHECK_ERROR(glDetachShader(**program, **vertexShader));
        MBGL_CHECK_ERROR(glDetachShader(**program, **fragmentShader));
    }
}
