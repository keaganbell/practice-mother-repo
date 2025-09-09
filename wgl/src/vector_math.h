#pragma once

#define PI 3.141592653589793

#define Vec2(x, y) vec2{ x, y }
#define Vec3(x, y, z) vec3{ x, y, z }
#define Vec4(x, y, z, w) vec4{ x, y, z, w }

struct vec2 {
    union {
        f32 Elements[2];
        struct {
            union {
                f32 x;
                f32 r;
            };
            union {
                f32 y;
                f32 g;
            };
        };
    };
};

struct vec3 {
    union {
        f32 Elements[3];
        struct {
            union {
                f32 x;
                f32 r;
            };
            union {
                f32 y;
                f32 g;
            };
            union {
                f32 z;
                f32 b;
            };
        };
    };
};

struct vec4 {
    union {
        f32 Elements[4];
        struct {
            union {
                f32 x;
                f32 r;
            };
            union {
                f32 y;
                f32 g;
            };
            union {
                f32 z;
                f32 b;
                f32 Width;
            };
            union {
                f32 w;
                f32 a;
                f32 Height;
            };
        };
    };
};

struct mat4 {
    f32 Elements[16];

    // Row-major mat4
    mat4() {
        for (i32 j = 0; j < 4; ++j) {
            for (i32 i = 0; i < 4; ++i) {
                Elements[j*4 + i] = i == j ? 1.0f : 0.0f;
            }
        }
    }
};

mat4 operator*(const mat4& A, const mat4& B) {
    mat4 Result;
    for (i32 j = 0; j < 4; ++j) {
        for (i32 i = 0; i < 4; ++i) {
            Result.Elements[j*4 + i] = 0.0f;
            for (i32 k = 0; k < 4; ++k) {
                Result.Elements[j*4 + i] += A.Elements[j*4 + k]*B.Elements[k*4 + i];
            }
        }
    }
    return Result;
}

vec3 operator+(const vec3& a, const vec3& b) {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
