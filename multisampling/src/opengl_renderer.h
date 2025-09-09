#pragma once

#define MAX_VERTEX_COUNT (1<<16)
#define MAX_INDEX_COUNT (1<<20)
struct opengl {
    vertex VertexData[MAX_VERTEX_COUNT];
    u32 IndexData[MAX_INDEX_COUNT];

    // MAIN RENDER PATH
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLuint MainProgram;

    // MULTISAMPLE
    b32 MultisampleEnabled;
    GLuint MultisampledFBO;
    GLuint MultisampledTexture;
    GLuint CompositeFBO;
    GLuint CompositeTexture;
    GLuint CompositeProgram;

    GLuint CompositeVAO;
    GLuint CompositeVBO;
};
