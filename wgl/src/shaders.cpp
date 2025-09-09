#pragma once

const char *VertexShaderSource = 
"#version 410 core\n"

"in vec3 _Position;\n"
"in vec2 _TexCoord;\n"
"in vec4 _Color;\n"

"uniform mat4 MVP;\n"

"out vec2 TexCoord;\n"
"out vec4 Color;\n"

"void main() {\n"
    "Color = _Color;\n"
    "TexCoord = _TexCoord;\n"
    "gl_Position = MVP*vec4(_Position, 1.0);\n"
"}\n";

const char *FragmentShaderSource = 
"#version 410 core\n"

"in vec2 TexCoord;\n"
"in vec4 Color;\n"

"out vec4 FragColor;\n"

"void main() {\n"
    "FragColor = vec4(Color.rg*TexCoord, 0.0, 1.0);\n"
"}\n";

static GLuint CreateShader(GLenum Type, const char *ShaderSource) {
    GLuint Shader = glCreateShader(Type);
    glShaderSource(Shader, 1, &ShaderSource, NULL);
    glCompileShader(Shader);

    GLint Succeeded;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Succeeded);
    if (Succeeded != GL_TRUE) {
        GLsizei Length = 0;
        GLchar Message[1024];
        glGetShaderInfoLog(Shader, 1024, &Length, Message);
        LERROR("%s", Message);
    }
    return Shader;
}

static GLuint CreateShaderProgram(GLuint VertexShader, GLuint FragmentShader) {
    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    glUseProgram(ShaderProgram);

    GLint Result;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Result);
    if (Result != GL_TRUE) {
        GLsizei Length = 0;
        GLchar Message[1024];
        glGetProgramInfoLog(ShaderProgram, 1024, &Length, Message);
        LERROR("%s", Message);
    }
    return ShaderProgram;
}
