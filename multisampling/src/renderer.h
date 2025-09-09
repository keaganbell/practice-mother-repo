struct camera {
    vec3 Position;
    vec3 Target;
    vec3 Up;
    f32 FOV;
    f32 Near;
    f32 Far;
};

struct vertex {
    vec3 Position;
};

struct render_group {
    vertex *Vertices;
    u32 MaxVertexCount;
    u32 VertexCount;

    u32 *Indices;
    u32 MaxIndexCount;
    u32 IndexCount;

    camera Camera;
};
