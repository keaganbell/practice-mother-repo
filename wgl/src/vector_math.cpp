#pragma once

// TODO: make this not literally copy 64 bytes 3 times...
static mat4 GetPerspectiveProjectionMatrix(f32 FOV, f32 Near, f32 Far, f32 Aspect) {
    mat4 Result = mat4();
    f32 Tan = tanf(FOV/2.0f);
    f32 a = 1.0f/(Tan*Aspect);
    f32 b = 1.0f/Tan;
#if 0
    f32 c = -(Far + Near)/(Near - Far);
    f32 d = 2.0f*Near*Far*(Near - Far);
#else
    f32 c = 2.0f/(Far - Near);
    f32 d = 1.0f;
#endif
    f32 e = 1.0f; // Left-handed coordinate system (right-handed would need -1.0f)
    f32 Matrix[16] = {
           a, 0.0f, 0.0f, 0.0f,
        0.0f,    b, 0.0f, 0.0f,
        0.0f, 0.0f,    c,   -d,
        0.0f, 0.0f,    e, 1.0f
    };
    memcpy(Result.Elements, Matrix, sizeof(Matrix));
    return Result;
}

// TODO: make this not literally copy 64 bytes so many times...
static mat4 GetRotationMatrixY(f32 Angle) {
    f32 a = cosf(Angle);
    f32 b = sinf(Angle);
    f32 Rotate[16] = {
           a, 0.0f,    b, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
          -b, 0.0f,    a, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    mat4 Result;
    memcpy(Result.Elements, Rotate, sizeof(Rotate));
    return Result;
}

static mat4 GetRotationMatrixX(f32 Angle) {
    f32 a = cosf(Angle);
    f32 b = sinf(Angle);
    f32 Rotate[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,    a,   -b, 0.0f,
        0.0f,    b,    a, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    mat4 Result;
    memcpy(Result.Elements, Rotate, sizeof(Rotate));
    return Result;
}
