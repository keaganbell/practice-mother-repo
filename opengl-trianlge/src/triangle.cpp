
static inline mat4 GetPerspectiveProjection() {
    f32 Aspect = (f32)GlobalClientWidth/GlobalClientHeight;
    f32 n = -1.f;
    f32 f = -100.f;
    f32 h = n*tanf((f32)PI/4.f); // half the height of the near plane
    f32 w = h*Aspect;

    f32 a = -1.f/w;
    f32 b = -1.f/h;
    f32 c = 2.f/(n - f);
    f32 d = (f + n)/(n - f);
    f32 e = 1.f;
    mat4 Result = {
          a, 0.f, 0.f, 0.f,
        0.f,   b, 0.f, 0.f,
        0.f, 0.f,   c,   d,
        0.f, 0.f,   e, 0.f
    };
    return Result;
}

static inline mat4 GetModelMatrix(f32 Angle) {
    f32 a = cosf(Angle);
    f32 b = sinf(Angle);
    mat4 RotateAboutY = {
           a, 0.0f,    b, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
          -b, 0.0f,    a, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    return RotateAboutY;
}

static vec3 CameraP = vec3(0.f, 0.f, 5.f);
static inline mat4 GetViewMatrix() {
    vec3 P = CameraP;
    vec3 T = vec3(0.f, 0.f, -1.f);
    vec3 U = vec3(0.f, 1.f,  0.f);
    vec3 R = Cross(T, U);

    f32 x = R.x*P.x + U.x*P.y + T.x*P.z;
    f32 y = R.y*P.x + U.y*P.y + T.y*P.z;
    f32 z = R.z*P.x + U.z*P.y + T.z*P.z;
    mat4 View = {
        R.x, U.x, T.x, -x,
        R.y, U.y, T.y, -y,
        R.z, U.z, T.z, -z,
        0.f, 0.f, 0.f, 1.f
    };
    return View;
}
static void UpdateAndRender(opengl *OpenGL, program_input *Input) {

    if (ButtonDown(Input, BUTTON_KEY_SPACE)) {
        CameraP.z += .1f;
    }
    if (ButtonDown(Input, BUTTON_KEY_SHIFT)) {
        CameraP.z -= .1f;
    }
    if (ButtonDown(Input, BUTTON_KEY_W)) {
        CameraP.y += .1f;
    }
    if (ButtonDown(Input, BUTTON_KEY_S)) {
        CameraP.y -= .1f;
    }
    if (ButtonDown(Input, BUTTON_KEY_A)) {
        CameraP.x -= .1f;
    }
    if (ButtonDown(Input, BUTTON_KEY_D)) {
        CameraP.x += .1f;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, GlobalClientWidth, GlobalClientHeight);

    vertex Vertices[4] = {};
    Vertices[0].Position = vec3(-.5f, -.5f, 0.f);
    Vertices[0].UV = vec2(0.0f, 0.0f);

    Vertices[1].Position = vec3( .5f, -.5f, 0.f);
    Vertices[1].UV = vec2(1.0f, 0.0f);

    Vertices[2].Position = vec3( .5f,  .5f, 0.f);
    Vertices[2].UV = vec2(1.0f, 1.0f);

    Vertices[3].Position = vec3(-.5f,  .5f, 0.f);
    Vertices[3].UV = vec2(0.0f, 1.0f);

    u32 Indices[] = { 0, 1, 2, 2, 3, 0 };

    OpenGL->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);
    OpenGL->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(Indices), Indices);

    static f32 t;
    t += 0.016667f;
    mat4 Model = GetModelMatrix(0.f);
    mat4 Projection = GetPerspectiveProjection();
    mat4 View = GetViewMatrix();
    mat4 PVM = Projection*View*Model;
    OpenGL->glUniformMatrix4fv(OpenGL->PVM, 1, GL_TRUE, PVM.Elements);

    glDrawElements(GL_TRIANGLES, ArrayCount(Indices), GL_UNSIGNED_INT, 0);
}
