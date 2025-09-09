
static void CreateRenderGroup(render_group *Group, render_group_description *Desc) {
    glGenVertexArrays(1, &Group->VAO);
    glBindVertexArray(Group->VAO);
    Group->Program = Desc->Program;
    Group->MaxVerticesCount = Desc->VerticesCount;
    // TODO: fix the indices count

    if (Desc->InlineVertices) {

        // VERTICES
        glGenBuffers(1, &Group->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Group->VBO);
        if (Desc->Vertices) {
            glBufferData(GL_ARRAY_BUFFER, Desc->VerticesCount*sizeof(vertex), Desc->Vertices, GL_STATIC_DRAW);
        }
        else {
            glBufferData(GL_ARRAY_BUFFER, Desc->VerticesCount*sizeof(vertex), NULL, GL_DYNAMIC_DRAW);
        }

        GLuint Position = glGetAttribLocation(Group->Program, "_Position");
        glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(offsetof(vertex, Position))); 
        glEnableVertexAttribArray(Position);

        GLuint TexCoord = glGetAttribLocation(Group->Program, "_TexCoord");
        glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(offsetof(vertex, TexCoord))); 
        glEnableVertexAttribArray(TexCoord);

        GLuint Color = glGetAttribLocation(Group->Program, "_Color");
        glVertexAttribPointer(Color, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(offsetof(vertex, Color)));
        glEnableVertexAttribArray(Color);

        // INDICES
        glGenBuffers(1, &Group->IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Group->IBO);
        if (Desc->Indices) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Desc->IndicesCount*sizeof(u32), Desc->Indices, GL_STATIC_DRAW);
        }
        else {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Desc->IndicesCount*sizeof(u32), NULL, GL_DYNAMIC_DRAW);
        }

        // UNIFORMS
        Group->MVP = glGetUniformLocation(Group->Program, "MVP");
    }
    else {
        LERROR("Separate buffers NYI.");
    }
    CheckGLError();
}

static void GetCubeIndices(u32 *Indices) {
    u32 Offset = 0;
    for (u32 i = 0; i < MAX_CUBE_COUNT; ++i) {
        Indices[i*36 + 0]  = 0 + Offset;
        Indices[i*36 + 1]  = 1 + Offset;
        Indices[i*36 + 2]  = 2 + Offset;
        Indices[i*36 + 3]  = 2 + Offset;
        Indices[i*36 + 4]  = 3 + Offset;
        Indices[i*36 + 5]  = 0 + Offset;

        Indices[i*36 + 6]  = 1 + Offset;
        Indices[i*36 + 7]  = 6 + Offset;
        Indices[i*36 + 8]  = 5 + Offset;
        Indices[i*36 + 9]  = 5 + Offset;
        Indices[i*36 + 10] = 2 + Offset;
        Indices[i*36 + 11] = 1 + Offset;

        Indices[i*36 + 12] = 3 + Offset;
        Indices[i*36 + 13] = 2 + Offset;
        Indices[i*36 + 14] = 5 + Offset;
        Indices[i*36 + 15] = 5 + Offset;
        Indices[i*36 + 16] = 4 + Offset;
        Indices[i*36 + 17] = 3 + Offset;

        Indices[i*36 + 18] = 4 + Offset;
        Indices[i*36 + 19] = 5 + Offset;
        Indices[i*36 + 20] = 6 + Offset;
        Indices[i*36 + 21] = 6 + Offset;
        Indices[i*36 + 22] = 7 + Offset;
        Indices[i*36 + 23] = 4 + Offset;

        Indices[i*36 + 24] = 0 + Offset;
        Indices[i*36 + 25] = 3 + Offset;
        Indices[i*36 + 26] = 4 + Offset;
        Indices[i*36 + 27] = 4 + Offset;
        Indices[i*36 + 28] = 7 + Offset;
        Indices[i*36 + 29] = 0 + Offset;

        Indices[i*36 + 30] = 0 + Offset;
        Indices[i*36 + 31] = 7 + Offset;
        Indices[i*36 + 32] = 6 + Offset;
        Indices[i*36 + 33] = 6 + Offset;
        Indices[i*36 + 34] = 1 + Offset;
        Indices[i*36 + 35] = 0 + Offset;
        Offset += 8;
    }
}

static vertex *PushVertex(render_group *Group) {
    vertex *Result = NULL;
    if (Group->VerticesCount < Group->MaxVerticesCount) {
        Result = Group->Vertices + Group->VerticesCount;
        ++Group->VerticesCount;
    }
    else {
        LERROR("Render group is out of vertices.");
    }
    return Result;
}

static void DrawCube(render_group *CubeGroup, cube *Cube) {

    vertex *V0 = PushVertex(CubeGroup);
    V0->Position = Vec3(-0.5f, -0.5f, -0.5f) + Cube->Position;
    V0->TexCoord = Vec2(0.0f, 0.0f);
    V0->Color    = Cube->Color;

    vertex *V1 = PushVertex(CubeGroup);
    V1->Position = Vec3(0.5f, -0.5f, -0.5f) + Cube->Position;
    V1->TexCoord = Vec2(1.0f, 0.0f);
    V1->Color    = Cube->Color;

    vertex *V2 = PushVertex(CubeGroup);
    V2->Position = Vec3(0.5f, 0.5f, -0.5f) + Cube->Position;
    V2->TexCoord = Vec2(1.0f, 1.0f);
    V2->Color    = Cube->Color;

    vertex *V3 = PushVertex(CubeGroup);
    V3->Position = Vec3(-0.5f, 0.5f, -0.5f) + Cube->Position;
    V3->TexCoord = Vec2(0.0f, 1.0f);
    V3->Color    = Cube->Color;

    vertex *V4 = PushVertex(CubeGroup);
    V4->Position = Vec3(-0.5f, 0.5f, 0.5f) + Cube->Position;
    V4->TexCoord = Vec2(0.0f, 1.0f);
    V4->Color    = Cube->Color;

    vertex *V5 = PushVertex(CubeGroup);
    V5->Position = Vec3(0.5f, 0.5f, 0.5f) + Cube->Position;
    V5->TexCoord = Vec2(1.0f, 1.0f);
    V5->Color    = Cube->Color;

    vertex *V6 = PushVertex(CubeGroup);
    V6->Position = Vec3(0.5f, -0.5f, 0.5f) + Cube->Position;
    V6->TexCoord = Vec2(1.0f, 0.0f);
    V6->Color    = Cube->Color;

    vertex *V7 = PushVertex(CubeGroup);
    V7->Position = Vec3(-0.5f, -0.5f, 0.5f) + Cube->Position;
    V7->TexCoord = Vec2(0.0f, 0.0f);
    V7->Color    = Cube->Color;
}

extern "C" {

void Initialize(program_memory *Memory) {
    program *Program = (program *)Memory->PermanentStorage;
    Program->PermanentArena = InitializeArena((u8 *)Memory->PermanentStorage, Memory->PermanentSize);
    Program->PermanentArena.Used += sizeof(*Program);

    Program->ScratchArena = InitializeArena((u8 *)Memory->FrameScratchStorage, Memory->FrameScratchSize);

    GLuint VertexShader = CreateShader(GL_VERTEX_SHADER, VertexShaderSource);
    GLuint FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentShaderSource);
    GLuint ShaderProgram = CreateShaderProgram(VertexShader, FragmentShader);

    u32 Indices[MAX_CUBE_COUNT*36] = {};
    GetCubeIndices(Indices);
    render_group_description CubeGroupDesc = {};
    CubeGroupDesc.Program = ShaderProgram;
    CubeGroupDesc.InlineVertices = true;
    CubeGroupDesc.VerticesCount = MAX_CUBE_COUNT*8;
    CubeGroupDesc.Indices = Indices;
    CubeGroupDesc.IndicesCount = MAX_CUBE_COUNT*36;
    CreateRenderGroup(&Program->CubeRenderGroup, &CubeGroupDesc);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void UpdateAndRender(program_memory *Memory) {
    program *Program = (program *)Memory->PermanentStorage;
    Program->ScratchArena = InitializeArena((u8 *)Memory->FrameScratchStorage, Memory->FrameScratchSize);

    render_group *CubeGroup = &Program->CubeRenderGroup;
    CubeGroup->Vertices = PushArray(&Program->ScratchArena, vertex, CubeGroup->MaxVerticesCount);
    CubeGroup->VerticesCount = 0;

    // TODO: update the positions based on a transform and model matrix
    cube Cube1 = {};
    Cube1.Position = Vec3(1.f, 0.f, 0.f);
    Cube1.Color = Vec4(1.f, 1.f, 1.f, 1.f);
    DrawCube(CubeGroup, &Cube1);

    cube Cube2 = {};
    Cube2.Position = Vec3(-1.f, 0.f, 0.f);
    Cube2.Color = Vec4(1.f, 1.f, 1.f, 1.f);
    DrawCube(CubeGroup, &Cube2);

    f32 AspectRatio = (f32)GlobalClientWidth/GlobalClientHeight;
    mat4 Projection = GetPerspectiveProjectionMatrix(PI/2.0f, 1.0f, 100.0f, AspectRatio);

    //static f32 t;
    //t += 0.0166667f;
    //mat4 Model = GetRotationMatrixY(t)*GetRotationMatrixX(t);

    mat4 View = mat4();
    View.Elements[3] = -3.0f;
    View.Elements[7] = -1.0f;
    View.Elements[11] = 3.0f;

    mat4 MVP = Projection*GetRotationMatrixY(1.f)*View;

    glBindVertexArray(Program->CubeRenderGroup.VAO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, CubeGroup->VerticesCount*sizeof(vertex), CubeGroup->Vertices);

    glUniformMatrix4fv(Program->CubeRenderGroup.MVP, 1, GL_TRUE, MVP.Elements);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, CubeGroup->VerticesCount/8*36, GL_UNSIGNED_INT, 0);
    CheckGLError();
}

}
