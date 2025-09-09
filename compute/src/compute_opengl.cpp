#pragma once

static inline void CheckOpenGLError(char *Context) {
    GLenum Error;
    while ((Error = glGetError()) != GL_NO_ERROR) {
        switch (Error) {
            case GL_INVALID_ENUM: {
                LERROR("OpenGL error in %s: GL_INVALID_ENUM", Context);
            } break;
            case GL_INVALID_VALUE: {
                LERROR("OpenGL error in %s: GL_INVALID_VALUE", Context);
            } break;
            case GL_INVALID_OPERATION: {
                LERROR("OpenGL error in %s: GL_INVALID_OPERATION", Context);
            } break;
            case GL_OUT_OF_MEMORY: {
                LERROR("OpenGL error in %s: GL_OUT_OF_MEMORY", Context);
            } break;
            default: {
                LERROR("OpenGL unkown error in %s", Context);
            }
        }
    }
}

static inline GLuint CreateShader(GLuint ShaderType, char *ShaderSource) {
    GLuint Shader = glCreateShader(ShaderType);
    glShaderSource(Shader, 1, &ShaderSource, 0);
    glCompileShader(Shader);

    GLint Success = 0;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetShaderInfoLog(Shader, sizeof(Buffer), 0, Buffer);
        LERROR("Shader (0x%x): %s", ShaderType, Buffer);
    }
    return Shader;
}

static inline GLuint CreateShaderProgram() {
    char *VertexSource = R"FOO(
    #version 310 es

    in vec3 _Position;
    in vec2 _UV;

    out vec2 UV;

    void main() {
        gl_Position = vec4(_Position, 1.0);
        UV = _UV;
    }
    )FOO";

    GLuint VertexShader = CreateShader(GL_VERTEX_SHADER, VertexSource);

    char *FragmentSource = R"FOO(
    #version 310 es
    precision highp float;

    in vec2 UV;
    uniform sampler2D Image;

    out vec4 FragColor;

    void main() {
        FragColor = texture(Image, UV);
    }
    )FOO";

    GLuint FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentSource);

    GLuint Program = glCreateProgram();
    glAttachShader(Program, VertexShader);
    glAttachShader(Program, FragmentShader);
    glLinkProgram(Program);

    GLint Success = 0;
    glGetProgramiv(Program, GL_LINK_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetProgramInfoLog(Program, sizeof(Buffer), 0, Buffer);
        LERROR("Program Link: %s\n", Buffer);
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

static inline GLuint CreateComputeProgram() {
    char *ComputeSource = R"FOO(
    #version 310 es

    layout(local_size_x = 16, local_size_y = 16) in;
    layout(rgba32f, binding = 0) uniform highp image2D Image;

    void main() {
        vec4 Pixel = vec4(1.0, 1.0, 1.0, 1.0);
        ivec2 PixelP = ivec2(gl_GlobalInvocationID.xy);
        imageStore(Image, PixelP, Pixel);
    }
    )FOO";

    GLuint ComputeShader = CreateShader(GL_COMPUTE_SHADER, ComputeSource);

    GLuint Program = glCreateProgram();
    glAttachShader(Program, ComputeShader);
    glLinkProgram(Program);

    GLint Success = 0;
    glGetProgramiv(Program, GL_LINK_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetProgramInfoLog(Program, sizeof(Buffer), 0, Buffer);
        LERROR("Program Link: %s\n", Buffer);
    }

    glValidateProgram(Program);
    glGetProgramiv(Program, GL_VALIDATE_STATUS, &Success);
    if (Success != GL_TRUE) {
        char Buffer[1024] = {};
        glGetProgramInfoLog(Program, sizeof(Buffer), 0, Buffer);
        LERROR("Program Validation: %s", Buffer);
    }

    glDeleteShader(ComputeShader);

    return Program;
}

#define WIDTH 256
#define HEIGHT 256
static inline GLuint CreateComputeTexture() {
    GLuint Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTextureStorage2D(Texture, 1, GL_RGBA32F, WIDTH, HEIGHT);

    glTextureParameteri(Texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return Texture;
}

static void InitOpenGL(opengl *OpenGL) {
    f32 QuadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &OpenGL->ResolveVAO);
    glGenBuffers(1, &OpenGL->ResolveVBO);

    glBindVertexArray(OpenGL->ResolveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->ResolveVBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3*sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    OpenGL->ComputeOutputTexture = CreateComputeTexture();

    OpenGL->ResolveProgram = CreateShaderProgram();
    OpenGL->ComputeProgram = CreateComputeProgram();
}

static void EndFrame(opengl *OpenGL) {
    glViewport(0, 0, GlobalScreenWidth, GlobalScreenHeight);
    glClearColor(1.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // Compute shader
    //
    glUseProgram(OpenGL->ComputeProgram);
    glBindImageTexture(0, OpenGL->ComputeOutputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute((WIDTH + 15) / 16, (WIDTH + 15) / 16, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    //
    // Render the final image
    //
    glUseProgram(OpenGL->ResolveProgram);
    glBindVertexArray(OpenGL->ResolveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->ResolveVBO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, OpenGL->ComputeOutputTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CheckOpenGLError("drawing resolved texture");

    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
