#pragma once

static void PopulateRenderGroup(render_group *Group) {
    Group->Vertices[0].Position = vec3( 0.f,  .5f, 0.f);
    Group->Vertices[1].Position = vec3(-.5f, -.5f, 0.f);
    Group->Vertices[2].Position = vec3( .5f, -.5f, 0.f);
    Group->VertexCount = 3;

    Group->Indices[0] = 0;
    Group->Indices[1] = 1;
    Group->Indices[2] = 2;
    Group->IndexCount = 3;
}
