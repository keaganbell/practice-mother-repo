#pragma once

#define MAX_VERTEX_COUNT (1<<16)
#define MAX_INDEX_COUNT (1<<20)

struct vertex {
    vec4 Color;
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    f32 TexID;
};

struct render_commands {
    u8 *PushBufferBase;
    u8 *PushBufferAt;
    size_t Cap;

    u32 MaxVertexCount;
    u32 VertexCount;
    vertex *Vertices;

    u32 MaxIndexCount;
    u32 IndexCount;
    u32 *Indices;
};
