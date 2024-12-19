#include "GLDebugEnable.h"

#include <exception>
#include <glad/glad.h>
#include <iostream>

void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei,
                                const GLchar* message, const void*)
{
    // clang-format off
    const char* type_string = "?";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:                   type_string = "error";               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     type_string = "deprecated behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      type_string = "undefined behavior";  break;
        case GL_DEBUG_TYPE_PORTABILITY:             type_string = "portability";         break;
        case GL_DEBUG_TYPE_MARKER:                  type_string = "marker";              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:              type_string = "push group";          break;
        case GL_DEBUG_TYPE_POP_GROUP:               type_string = "pop group";           break;
        case GL_DEBUG_TYPE_OTHER:                   type_string = "other";               break;
    }

    const char* severity_string = "?";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            severity_string = "high";    break;
        case GL_DEBUG_SEVERITY_MEDIUM:          severity_string = "medium";  break;
        case GL_DEBUG_SEVERITY_LOW:             severity_string = "low";     break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:                                return;
    }

    const char* source_string = "?";
    switch (source) {
        case GL_DEBUG_SOURCE_API:               source_string = "API";               break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     source_string = "window system";     break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   source_string = "shader compiler";   break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       source_string = "third party";       break;
        case GL_DEBUG_SOURCE_APPLICATION:       source_string = "app";               break;
        case GL_DEBUG_SOURCE_OTHER:             source_string = "other";             break;
    }
    // clang-format on
    std::println(std::cerr, "OpenGL message.\n Type: {}\nnSeverity: {}\nnSource: {}\nnMessage: {}",
               type_string, severity_string, source_string, message);
}

void init_opengl_debugging()
{
#ifndef __APPLE__
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
    glDebugMessageCallback(glDebugCallback, NULL);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
}