#pragma once

struct vertex {
    vec3 Position;
    vec2 TexCoord;
    vec4 Color;
};

struct cube {
    vec3 Position;
    vec4 Color;
};

#define MAX_CUBE_COUNT 1024
struct render_group {
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    GLuint Program;
    GLuint MVP;

    // Must be contiguous
    vertex *Vertices;
    u32 VerticesCount;
    u32 MaxVerticesCount;
};

struct render_group_description {
    GLuint Program;
    b32 InlineVertices;

    vertex *Vertices;
    u32 VerticesCount;

    u32 *Indices;
    u32 IndicesCount;

    vec3 *Positions;
    vec2 *TexCoords;
    vec4 *Colors;
    vec3 *Normals;
};

struct program {
    memory_arena PermanentArena;
    memory_arena ScratchArena;

    render_group CubeRenderGroup;

    mat4 Projection;
};

