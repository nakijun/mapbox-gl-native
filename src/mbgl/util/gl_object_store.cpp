#include <mbgl/util/gl_object_store.hpp>

#include <mbgl/util/thread.hpp>
#include <mbgl/geometry/vao.hpp>
#include <mbgl/gl/gl.hpp>

#include <cassert>

namespace mbgl {
namespace util {

void GLObjectStore::abandonVAO(GLuint vao) {
    assert(ThreadContext::currentlyOn(ThreadType::Map));
    abandonedVAOs.emplace_back(vao);
}

void GLObjectStore::abandonBuffer(gl::BufferPtr buffer) {
    assert(ThreadContext::currentlyOn(ThreadType::Map));
    abandonedBuffers.emplace_back(std::move(buffer));
}

void GLObjectStore::abandonTexture(GLuint texture) {
    assert(ThreadContext::currentlyOn(ThreadType::Map));
    abandonedTextures.emplace_back(texture);
}

void GLObjectStore::performCleanup() {
    assert(ThreadContext::currentlyOn(ThreadType::Map));

    if (!abandonedVAOs.empty()) {
        MBGL_CHECK_ERROR(VertexArrayObject::Delete(static_cast<GLsizei>(abandonedVAOs.size()),
                                                   abandonedVAOs.data()));
        abandonedVAOs.clear();
    }

    if (!abandonedTextures.empty()) {
        MBGL_CHECK_ERROR(glDeleteTextures(static_cast<GLsizei>(abandonedTextures.size()),
                                          abandonedTextures.data()));
        abandonedTextures.clear();
    }

    abandonedBuffers.clear();
}

} // namespace util
} // namespace mbgl
