#pragma once

// NOTE: HH uses a separate program for what maybe could be called
// a render pass: 
//  - opengl_program_common
//      * handle
//      * attrib locations (position, normal, uv, ...)
//      * samplers[16]
//
//  - zbias_program
//      * opengl_program_common
//      * transform id
//      * camera position
//      * fog direction, color, distance...
//      * clipping alpha stuff (i think this was for moving up and down stairs)
//  
//  IDEA
//  - postprocess_program
//      * opengl_program_common
//      * raytracing data...

// Basically create a program for each stage of the game's render pipeline.
// Maybe could say that there are 5 render passes per frame? Render to each
// render target (framebuffer) and then composite them on the screen.
//
// struct opengl {
//     opengl_framebuffer ResolveFramebuffer;
//     opengl_framebuffer DepthPeelBuffer; //[16];
//     opengl_framebuffer DepthPeelResolveBuffer[16];
//
//     zbias_program ZBiasNoDepthPeel; // NOTE(casey): Pass 0
//     zbias_program ZBiasDepthPeel; // NOTE(casey): Passes 1 through n
//     opengl_program_common PeelComposite; // NOTE(casey): Composite all passes
//     opengl_program_common FinalStretch;
//     opengl_program_common ResolveMultisample;
// }

struct opengl_info {
    char *Vendor;
    char *Renderer;
    char *Version;
    char *Extensions;
    char *ShadingLanguageVersion;
    // TODO: add more info about which
    // extensions are supported.
};

struct opengl_base_program {
    GLuint Handle;

    // Vertex attributes
    GLuint PositionID;
    GLuint NormalID;
    GLuint TexCoordID;
    GLuint ColorID;
    GLuint TextureID;

    // TODO: samplers?
};

struct opengl {
    // platform_renderer header;
    // game_render_settings CurrentSettings;
    // GLuint TextureHandles...;

    // NOTE: HH just has a single VBO and packs the vertices inline
    //u8 PushBufferMemory[65536];
    //textured_vertex *VertexArray;
    //u16 *IndexArray;
    //renderer_texture *BitmapArray;

    //GLuint VertexBuffer;
    //GLuint IndexBuffer;
    //GLuint ScreenFillVertexBuffer;
    //GLuint LightMapDebugBuffer;
    //
    //u32 MaxQuadTextureCount;
    //u32 MaxTextureCount;
    //u32 MaxVertexCount;
    //u32 MaxIndexCount;

    //game_render_commands RenderCommands;

    opengl_base_program DefaultProgram;

#define OpenGLFunction(Name) type_##Name *Name

    OpenGLFunction(glCreateShader);
    OpenGLFunction(glShaderSource);
    OpenGLFunction(glCompileShader);
    OpenGLFunction(glGetShaderInfoLog);
    OpenGLFunction(glGetShaderiv);
    OpenGLFunction(glDeleteShader);

    OpenGLFunction(glCreateProgram);
    OpenGLFunction(glAttachShader);
    OpenGLFunction(glLinkProgram);
    OpenGLFunction(glUseProgram);
    OpenGLFunction(glGetProgramInfoLog);
    OpenGLFunction(glGetProgramiv);
    OpenGLFunction(glDeleteProgram);

    OpenGLFunction(glGetUniformLocation);
    OpenGLFunction(glUniformMatrix4fv);
    OpenGLFunction(glUniform4fv);
    OpenGLFunction(glUniform1i);
    OpenGLFunction(glUniform1f);
    OpenGLFunction(glUniform2fv);
    OpenGLFunction(glUniform3fv);

    OpenGLFunction(glGenVertexArrays);
    OpenGLFunction(glBindVertexArray);
    OpenGLFunction(glGenBuffers);
    OpenGLFunction(glBindBuffer);
    OpenGLFunction(glBufferData);
    OpenGLFunction(glBufferSubData);

    OpenGLFunction(glGetAttribLocation);
    OpenGLFunction(glVertexAttribPointer);
    OpenGLFunction(glVertexAttribIPointer);
    OpenGLFunction(glEnableVertexAttribArray);
    OpenGLFunction(glDisableVertexAttribArray);
};
