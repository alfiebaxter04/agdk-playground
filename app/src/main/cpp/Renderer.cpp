#include "Renderer.h"
#include "logging.h"

#include <cassert>
#include <GLES/egl.h>
#include <GLES3/gl3.h>

constexpr auto VERT_CODE = R"(#version 300 es
precision mediump float;

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec3 a_color;

out vec3 color;

void main() {
  gl_Position = vec4(a_pos, 0.0, 1.0);
  color = a_color;
}
)";
constexpr auto FRAG_CODE = R"(#version 300 es
precision mediump float;

in vec3 color;
out vec4 frag_color;

void main() {
  frag_color = vec4(color, 1.0);
}
)";

Renderer::Renderer(android_app *app) {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display);

    auto res = eglInitialize(display, nullptr, nullptr);
    assert(res == EGL_TRUE);

    {
        EGLint attribs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RED_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_NONE,
        };

        EGLint num_configs;
        eglChooseConfig(display, attribs, &config, 1, &num_configs);
        assert(num_configs == 1);
    }

    surface = eglCreateWindowSurface(display, config, app->window, nullptr);
    assert(surface != EGL_NO_SURFACE);

    {
        EGLint attribs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE,
        };
        context = eglCreateContext(display, config, nullptr, attribs);
        assert (context != EGL_NO_CONTEXT);
    }

    res = eglMakeCurrent(display, surface, surface, context);
    assert(res);

    LOGI("EGL initialization complete");

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    float vertices[] = {
            0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    auto compile_shader = [](GLenum type, const char *src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            LOGE("Failed to compile shader! Log: %s", info_log);
        }

        return shader;
    };

    GLuint vert = compile_shader(GL_VERTEX_SHADER, VERT_CODE);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, FRAG_CODE);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, info_log);
        LOGE("Failed to link shader! Log: %s", info_log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Renderer::~Renderer() {
    glDeleteProgram(program);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

void Renderer::do_frame() {
    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}