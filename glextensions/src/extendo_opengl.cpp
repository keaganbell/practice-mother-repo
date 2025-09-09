#pragma once

static opengl_info GetOpenGLInfo() {
    opengl_info Result = {};
    Result.Vendor = (char *)glGetString(GL_VENDOR);
    Result.Renderer = (char *)glGetString(GL_RENDERER);
    Result.Version = (char *)glGetString(GL_VERSION);
    Result.Extensions = (char *)glGetString(GL_EXTENSIONS);
    Result.ShadingLanguageVersion = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    return Result;
}

static void InitOpenGL(opengl *OpenGL, opengl_info Info) {
}
