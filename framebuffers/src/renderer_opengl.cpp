#pragma once
#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

static void InitOpenGL(opengl *OpenGL) {

    // BATCH RENDERER
    glGenFramebuffers(1, &OpenGL->FBO);
    glGenTextures(1, &OpenGL->MultisampledTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, OpenGL->MultisampledTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, TARGET_WIDTH, TARGET_HEIGHT, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, OpenGL->MultisampledTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LERROR("Framebuffer is not complete!");
    }
    //glGenVertexArrays(1, &OpenGL->BatchVAO);
    //glBindVertexArray(OpenGL->BatchVAO);

    // FINAL COMPOSITE IMAGE
    glGenVertexArrays(1, &OpenGL->CompositeVAO);
    glGenTextures(1, &OpenGL->CompositeTexture);
    glBindTexture(GL_TEXTURE_2D, OpenGL->CompositeTexture);

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
    glEnableVertexAttribArray(0); // hard code this in the shader i guess
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

static render_commands OpenGLBeginFrame() {
    render_commands Result = {};
    return Result;
}

static void OpenGLBeginUseProgram(default_shader_program Program) {
}

static void OpenGLEndUseProgram(default_shader_program Program) {
}

static void OpenGLBeginUseProgram(final_composite_program Program) {
}

static void OpenGLEndUseProgram(final_composite_program Program) {
}

static void OpenGLEndFrame(opengl *OpenGL, render_commands *Commands) {

    //
    // render to the framebuffer
    //
    OpenGLBeginUseProgram(OpenGL->DefaultProgram);
    glBindVertexArray(OpenGL->BatchVAO);
    glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->FBO);
    glViewport(0, 0, TARGET_WIDTH, TARGET_HEIGHT);
    glClearColor(1.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    //
    // do the actual rendering
    //
    OpenGLEndUseProgram(OpenGL->DefaultProgram);

    //
    // render the framebuffer to the screen
    //
    OpenGLBeginUseProgram(OpenGL->FinalCompositeProgram);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGL->MultisampledTexture); // Source (multisampled)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OpenGL->CompositeTexture); // Destination (regular)
    glBlitFramebuffer(0, 0, TARGET_WIDTH, TARGET_HEIGHT, 0, 0, GlobalScreenWidth, GlobalScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindVertexArray(OpenGL->CompositeVAO);
    glViewport(0, 0, GlobalScreenWidth, GlobalScreenHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    OpenGLEndUseProgram(OpenGL->FinalCompositeProgram);

    glBindVertexArray(0);
}
