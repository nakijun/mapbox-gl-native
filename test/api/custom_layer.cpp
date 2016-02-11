#include "../fixtures/util.hpp"

#include <mbgl/map/map.hpp>
#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/platform/default/headless_view.hpp>
#include <mbgl/storage/online_file_source.hpp>
#include <mbgl/layer/custom_layer.hpp>
#include <mbgl/util/io.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/gl/gl_handle.hpp>

using namespace mbgl;

static const GLchar * vertexShaderSource = "attribute vec2 a_pos; void main() { gl_Position = vec4(a_pos, 0, 1); }";
static const GLchar * fragmentShaderSource = "void main() { gl_FragColor = vec4(0, 1, 0, 1); }";

class TestLayer {
public:
    ~TestLayer() {
        if (program) {
            MBGL_CHECK_ERROR(glDetachShader(**program, **vertexShader));
            MBGL_CHECK_ERROR(glDetachShader(**program, **fragmentShader));
        }
    }

    void initialize() {
        program = std::make_unique<gl::ProgramHandle>();
        vertexShader = std::make_unique<gl::ShaderHandle>(GL_VERTEX_SHADER);
        fragmentShader = std::make_unique<gl::ShaderHandle>(GL_FRAGMENT_SHADER);
        buffer = std::make_unique<gl::BufferHandle>();

        MBGL_CHECK_ERROR(glShaderSource(**vertexShader, 1, &vertexShaderSource, nullptr));
        MBGL_CHECK_ERROR(glCompileShader(**vertexShader));
        MBGL_CHECK_ERROR(glAttachShader(**program, **vertexShader));
        MBGL_CHECK_ERROR(glShaderSource(**fragmentShader, 1, &fragmentShaderSource, nullptr));
        MBGL_CHECK_ERROR(glCompileShader(**fragmentShader));
        MBGL_CHECK_ERROR(glAttachShader(**program, **fragmentShader));
        MBGL_CHECK_ERROR(glLinkProgram(**program));
        a_pos = glGetAttribLocation(**program, "a_pos");

        GLfloat background[] = { -1,-1, 1,-1, -1,1, 1,1 };
        MBGL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, **buffer));
        MBGL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), background, GL_STATIC_DRAW));
    }

    void render() {
        MBGL_CHECK_ERROR(glUseProgram(**program));
        MBGL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, **buffer));
        MBGL_CHECK_ERROR(glEnableVertexAttribArray(a_pos));
        MBGL_CHECK_ERROR(glVertexAttribPointer(a_pos, 2, GL_FLOAT, GL_FALSE, 0, NULL));
        MBGL_CHECK_ERROR(glDisable(GL_STENCIL_TEST));
        MBGL_CHECK_ERROR(glDisable(GL_DEPTH_TEST));
        MBGL_CHECK_ERROR(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }

    gl::ProgramPtr program;
    gl::ShaderPtr vertexShader;
    gl::ShaderPtr fragmentShader;
    gl::BufferPtr buffer;
    GLuint a_pos = 0;
};

TEST(CustomLayer, Basic) {
    auto display = std::make_shared<mbgl::HeadlessDisplay>();
    HeadlessView view(display, 1);
    OnlineFileSource fileSource(nullptr);

    Map map(view, fileSource, MapMode::Still);
    map.setStyleJSON(util::read_file("test/fixtures/api/empty.json"), "");
    map.addCustomLayer(
        "custom",
        [] (void* context) {
            reinterpret_cast<TestLayer*>(context)->initialize();
        },
        [] (void* context, const CustomLayerRenderParameters&) {
            reinterpret_cast<TestLayer*>(context)->render();
        },
        [] (void* context) {
            delete reinterpret_cast<TestLayer*>(context);
        }, new TestLayer());

    test::checkImage("test/fixtures/custom_layer/basic", test::render(map));
}
