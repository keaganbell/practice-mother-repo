#pragma once

static GLuint CompileShaderProgram(opengl *OpenGL, char *VertexCode, char *FragmentCode) {
    GLuint VertexShader = OpenGL->glCreateShader(GL_VERTEX_SHADER);
    OpenGL->glShaderSource(VertexShader, 1, &VertexCode, 0);
    OpenGL->glCompileShader(VertexShader);
    GLint Success;
    OpenGL->glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        OpenGL->glGetShaderInfoLog(VertexShader, sizeof(Buffer), 0, Buffer);
        OutputDebugString(Buffer);
    }

    GLuint FragmentShader = OpenGL->glCreateShader(GL_FRAGMENT_SHADER);
    OpenGL->glShaderSource(FragmentShader, 1, &FragmentCode, 0);
    OpenGL->glCompileShader(FragmentShader);
    OpenGL->glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        OpenGL->glGetShaderInfoLog(FragmentShader, sizeof(Buffer), 0, Buffer);
        OutputDebugString(Buffer);
    }

    GLuint Result = OpenGL->glCreateProgram();
    OpenGL->glAttachShader(Result, VertexShader);
    OpenGL->glAttachShader(Result, FragmentShader);
    OpenGL->glLinkProgram(Result);
    OpenGL->glValidateProgram(Result);
    OpenGL->glGetProgramiv(Result, GL_LINK_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        OpenGL->glGetProgramInfoLog(Result, sizeof(Buffer), 0, Buffer);
        OutputDebugString(Buffer);
    }

    OpenGL->glDeleteShader(VertexShader);
    OpenGL->glDeleteShader(FragmentShader);

    return Result;
}

static GLuint CreateDefaultProgram(opengl *OpenGL) {
    char *VertexSource = R"FOO(
    #version 330 core

    in vec3 _Position;
    in vec2 _UV;

    uniform mat4 PVM;

    out vec2 UV;

    void main() {
        UV = _UV;
        gl_Position = PVM*vec4(_Position, 1.0);
    }
    )FOO";


    char *FragmentSource = R"FOO(
    #version 330 core

    in vec2 UV;

    out vec4 FragColor;

    void main() {
        FragColor = vec4(UV, 0.0, 1.0);
    }
    )FOO";

    GLuint Program = CompileShaderProgram(OpenGL, VertexSource, FragmentSource);

    OpenGL->PVM = OpenGL->glGetUniformLocation(Program, "PVM");

    return Program;
}

static void InitOpenGL(opengl *OpenGL) {
    // compile and create all the programs and buffer objects
    u64 MaxQuadCount = (1<<20); // 1 Mi Quads
    u64 MaxVertexCount = MaxQuadCount*4;
    u64 MaxIndexCount = MaxQuadCount*6;
    OpenGL->Vertices = (vertex *)malloc(MaxVertexCount*sizeof(vertex));
    OpenGL->Indices = (u32 *)malloc(MaxIndexCount*sizeof(u32));

    GLuint Program = CreateDefaultProgram(OpenGL);
    OpenGL->glUseProgram(Program);

    OpenGL->glGenVertexArrays(1, &OpenGL->VAO);
    OpenGL->glBindVertexArray(OpenGL->VAO);

    OpenGL->glGenBuffers(1, &OpenGL->VBO);
    OpenGL->glBindBuffer(GL_ARRAY_BUFFER, OpenGL->VBO);
    OpenGL->glBufferData(GL_ARRAY_BUFFER, MaxVertexCount, NULL, GL_STREAM_DRAW);

    OpenGL->glGenBuffers(1, &OpenGL->IBO);
    OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL->IBO);
    OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount, NULL, GL_STREAM_DRAW);

    GLuint PositionID = OpenGL->glGetAttribLocation(Program, "_Position");
    OpenGL->glEnableVertexAttribArray(PositionID);
    OpenGL->glVertexAttribPointer(PositionID, 3, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, Position));

    GLuint TexCoordID = OpenGL->glGetAttribLocation(Program, "_UV");
    OpenGL->glEnableVertexAttribArray(TexCoordID);
    OpenGL->glVertexAttribPointer(TexCoordID, 2, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, UV));
}
