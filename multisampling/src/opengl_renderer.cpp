#pragma once

#define GL_DEBUG_SEVERITY_HIGH_ARB        0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB      0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB         0x9148
void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB: {
            PlatformDebugPrint("ERROR: OpenGL (0x%x): %s\n", source, message);
        } break;

        case GL_DEBUG_SEVERITY_MEDIUM_ARB: {
            PlatformDebugPrint("WARN:  OpenGL (0x%x): %s\n", source, message);
        } break;

        case GL_DEBUG_SEVERITY_LOW_ARB: {
            PlatformDebugPrint("INFO:  OpenGL (0x%x): %s\n", source, message);
        } break;
    }
}

static GLuint CompileShader(char *VertexShaderSource, char *FragmentShaderSource) {
    GLint Success;

    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
    glCompileShader(VertexShader);
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetShaderInfoLog(VertexShader, sizeof(Buffer), 0, Buffer);
        LERROR("Vertex Shader: %s", Buffer);
    }

    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetShaderInfoLog(FragmentShader, sizeof(Buffer), 0, Buffer);
        LERROR("Fragment Shader: %s", Buffer);
    }

    GLuint Program = glCreateProgram();
    glAttachShader(Program, VertexShader);
    glAttachShader(Program, FragmentShader);
    glLinkProgram(Program);
    glGetProgramiv(Program, GL_LINK_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetProgramInfoLog(Program, sizeof(Buffer), 0, Buffer);
        LERROR("Program Link: %s", Buffer);
    }

    glValidateProgram(Program);
    glGetProgramiv(Program, GL_VALIDATE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetProgramInfoLog(Program, sizeof(Buffer), 0, Buffer);
        LERROR("Program Validation: %s", Buffer);
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return Program;
}

static void CreateMainRenderProgram(opengl *OpenGL) {
    char *VertexSource = R"FOO(
    #version 330 core

    layout(location = 0) in vec3 _Position;

    void main() {
        gl_Position = vec4(_Position, 1.0);
    }

    )FOO";

    char *FragmentSource = R"FOO(
    #version 330 core

    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0);
    }

    )FOO";

    GLuint Handle = CompileShader(VertexSource, FragmentSource);
    glUseProgram(Handle);
    glBindAttribLocation(Handle, 0, "_Position");

    OpenGL->MainProgram = Handle;
    glUseProgram(0);
}

static void CreateFinalCompositeProgram(opengl *OpenGL) {
    char *VertexSource = R"FOO(
    #version 330 core

    layout(location = 0) in vec2 _Position;
    layout(location = 1) in vec2 _UV;

    out vec2 UV;

    void main() {
        UV = _UV;
        gl_Position = vec4(_Position, 0.0, 1.0);
    }

    )FOO";

    char *FragmentSource = R"FOO(
    #version 330 core

    in vec2 UV;

    uniform sampler2D Tex;

    out vec4 FragColor;

    void main() {
        FragColor = texture(Tex, UV);
    }

    )FOO";

    GLuint Handle = CompileShader(VertexSource, FragmentSource);
    glUseProgram(Handle);
    glBindAttribLocation(Handle, 0, "_Position");
    glBindAttribLocation(Handle, 1, "_UV");

    OpenGL->CompositeProgram = Handle;
    glUseProgram(0);
}

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

static void InitOpenGL(opengl *OpenGL) {
    glDebugMessageCallback(DebugCallback, NULL);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // MAIN RENDER PATH
    glGenVertexArrays(1, &OpenGL->VAO);
    glBindVertexArray(OpenGL->VAO);

    glGenBuffers(1, &OpenGL->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(OpenGL->VertexData), 0, GL_STREAM_DRAW);

    glGenBuffers(1, &OpenGL->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(OpenGL->IndexData), 0, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, Position));
    glEnableVertexAttribArray(0);

    CreateMainRenderProgram(OpenGL);

    // MULTISAMPLE COMPOSITE
    OpenGL->MultisampleEnabled = true;
    glGenFramebuffers(1, &OpenGL->MultisampledFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->MultisampledFBO);
    glGenTextures(1, &OpenGL->MultisampledTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, OpenGL->MultisampledTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGBA8, TARGET_WIDTH, TARGET_HEIGHT, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, OpenGL->MultisampledTexture, 0);
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        LERROR("Framebuffer incomplete: 0x%x", Status);
    }

    glGenFramebuffers(1, &OpenGL->CompositeFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->CompositeFBO);
    glGenTextures(1, &OpenGL->CompositeTexture);
    glBindTexture(GL_TEXTURE_2D, OpenGL->CompositeTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TARGET_WIDTH, TARGET_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, OpenGL->CompositeTexture, 0);
    Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        LERROR("Framebuffer incomplete: 0x%x", Status);
    }

    f32 QuadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
    };
    glGenVertexArrays(1, &OpenGL->CompositeVAO);
    glBindVertexArray(OpenGL->CompositeVAO);

    glGenBuffers(1, &OpenGL->CompositeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->CompositeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2 * sizeof(float)));

    CreateFinalCompositeProgram(OpenGL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static render_group BeginFrame(opengl *OpenGL) {
    render_group Result = {};
    Result.Vertices = OpenGL->VertexData;
    Result.MaxVertexCount = ArrayCount(OpenGL->VertexData);
    Result.Indices = OpenGL->IndexData;
    Result.MaxIndexCount = ArrayCount(OpenGL->IndexData);
    return Result;
}

static void EndFrame(opengl *OpenGL, render_group *Group) {

    if (OpenGL->MultisampleEnabled) {

        //
        // Initial Render
        //
        glBindVertexArray(OpenGL->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, OpenGL->VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL->IBO);
        glUseProgram(OpenGL->MainProgram);
        glEnableVertexAttribArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->MultisampledFBO);

        glViewport(0, 0, TARGET_WIDTH, TARGET_HEIGHT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*Group->VertexCount, Group->Vertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(u32)*Group->IndexCount, Group->Indices);
        glDrawElements(GL_TRIANGLES, Group->IndexCount, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //
        // Final Composite
        //

        glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGL->MultisampledFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OpenGL->CompositeFBO);
        glBlitFramebuffer(0, 0, TARGET_WIDTH, TARGET_HEIGHT,
                          0, 0, TARGET_WIDTH, TARGET_HEIGHT,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindVertexArray(OpenGL->CompositeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, OpenGL->CompositeVBO);
        glUseProgram(OpenGL->CompositeProgram);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glViewport(0, 0, GlobalScreenWidth, GlobalScreenHeight);
        glClearColor(1.0f, 0.0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    else {
        glBindVertexArray(OpenGL->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, OpenGL->VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL->IBO);
        glUseProgram(OpenGL->MainProgram);
        glEnableVertexAttribArray(0);

        glViewport(0, 0, GlobalScreenWidth, GlobalScreenHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*Group->VertexCount, Group->Vertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(u32)*Group->IndexCount, Group->Indices);
        glDrawElements(GL_TRIANGLES, Group->IndexCount, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}
