#pragma once

#define Win32GetOpenGLFunction(Name) OpenGL->Name = (type_##Name *)wglGetProcAddress(#Name)

static opengl *WindowsSetupOpenGL(HDC DC) {
    opengl *OpenGL = NULL;

    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    DesiredPixelFormat.nVersion   = 1;
    DesiredPixelFormat.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.cColorBits = 24;
    DesiredPixelFormat.cAlphaBits = 8;
    DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

    i32 SuggestedPixelFormatIndex = ChoosePixelFormat(DC, &DesiredPixelFormat);
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat = {};
    DescribePixelFormat(DC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
    SetPixelFormat(DC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

    HGLRC RC = wglCreateContext(DC);
    if (wglMakeCurrent(DC, RC)) {

        OpenGL = (opengl *)malloc(sizeof(opengl));

        Win32GetOpenGLFunction(glCreateShader);
        Win32GetOpenGLFunction(glShaderSource);
        Win32GetOpenGLFunction(glCompileShader);
        Win32GetOpenGLFunction(glGetShaderInfoLog);
        Win32GetOpenGLFunction(glGetShaderiv);
        Win32GetOpenGLFunction(glDeleteShader);

        Win32GetOpenGLFunction(glCreateProgram);
        Win32GetOpenGLFunction(glAttachShader);
        Win32GetOpenGLFunction(glLinkProgram);
        Win32GetOpenGLFunction(glUseProgram);
        Win32GetOpenGLFunction(glValidateProgram);
        Win32GetOpenGLFunction(glGetProgramInfoLog);
        Win32GetOpenGLFunction(glGetProgramiv);
        Win32GetOpenGLFunction(glDeleteProgram);

        Win32GetOpenGLFunction(glGetUniformLocation);
        Win32GetOpenGLFunction(glUniformMatrix4fv);
        Win32GetOpenGLFunction(glUniform4fv);
        Win32GetOpenGLFunction(glUniform1i);
        Win32GetOpenGLFunction(glUniform1f);
        Win32GetOpenGLFunction(glUniform2fv);
        Win32GetOpenGLFunction(glUniform3fv);

        Win32GetOpenGLFunction(glGenVertexArrays);
        Win32GetOpenGLFunction(glBindVertexArray);
        Win32GetOpenGLFunction(glGenBuffers);
        Win32GetOpenGLFunction(glBindBuffer);
        Win32GetOpenGLFunction(glBufferData);
        Win32GetOpenGLFunction(glBufferSubData);

        Win32GetOpenGLFunction(glGetAttribLocation);
        Win32GetOpenGLFunction(glVertexAttribPointer);
        Win32GetOpenGLFunction(glVertexAttribIPointer);
        Win32GetOpenGLFunction(glEnableVertexAttribArray);
        Win32GetOpenGLFunction(glDisableVertexAttribArray);

        InitOpenGL(OpenGL);
    }

    return OpenGL;
}
