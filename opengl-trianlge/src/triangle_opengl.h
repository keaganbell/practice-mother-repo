#pragma once

#define RENDERER_PUSH_BUFFER_MAX_SIZE (1 << 16) // 64 kiB
struct opengl {

    vertex *Vertices;
    u32 *Indices;

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    GLuint ShaderProgram;
    GLuint PVM;

#define OpenGLFunction(Name) type_##Name *Name

    OpenGLFunction(glCreateShader);
    OpenGLFunction(glShaderSource);
    OpenGLFunction(glCompileShader);
    OpenGLFunction(glGetShaderInfoLog);
    OpenGLFunction(glGetShaderiv);
    OpenGLFunction(glDeleteShader);

    OpenGLFunction(glCreateProgram);
    OpenGLFunction(glAttachShader);
    OpenGLFunction(glLinkProgram);
    OpenGLFunction(glUseProgram);
    OpenGLFunction(glValidateProgram);
    OpenGLFunction(glGetProgramInfoLog);
    OpenGLFunction(glGetProgramiv);
    OpenGLFunction(glDeleteProgram);

    OpenGLFunction(glGetUniformLocation);
    OpenGLFunction(glUniformMatrix4fv);
    OpenGLFunction(glUniform4fv);
    OpenGLFunction(glUniform1i);
    OpenGLFunction(glUniform1f);
    OpenGLFunction(glUniform2fv);
    OpenGLFunction(glUniform3fv);

    OpenGLFunction(glGenVertexArrays);
    OpenGLFunction(glBindVertexArray);
    OpenGLFunction(glGenBuffers);
    OpenGLFunction(glBindBuffer);
    OpenGLFunction(glBufferData);
    OpenGLFunction(glBufferSubData);

    OpenGLFunction(glGetAttribLocation);
    OpenGLFunction(glVertexAttribPointer);
    OpenGLFunction(glVertexAttribIPointer);
    OpenGLFunction(glEnableVertexAttribArray);
    OpenGLFunction(glDisableVertexAttribArray);
};
