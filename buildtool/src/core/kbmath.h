#ifndef kbmath_h
#define kbmath_h

#define vec2(x, y) (vec2) { x, y }
union vec2 {
    struct { f32 x;     f32 y; };
    struct { f32 u;     f32 v; };
    struct { f32 width; f32 height; };
    f32 e[2];
};

#define VEC3_ZERO vec3(0.0f, 0.0f, 0.0f)
#define vec3(x, y, z) (vec3) { x, y, z }
union vec3 {
    struct { f32 x; f32 y; f32 z; };
    struct { f32 r; f32 g; f32 b; };
    f32 e[3];
};

#define vec4(x, y, z, w) (vec4) { x, y, z, w }
union vec4 {
    struct { f32 x; f32 y; f32 z; f32 w; };
    struct { f32 r; f32 g; f32 b; f32 a; };
    struct { vec3 col; f32 al; };
    struct { vec3 pos; f32 wp; };
    f32 e[4];
};

#define vec2i(x, y) (vec2i) { x, y }
union vec2i {
    struct { i32 x;     i32 y; };
    struct { i32 width; i32 height; };
    i32 e[2];
};

struct mat3 {
    f32 e[9];
};

struct mat4 {
    f32 e[16];
};

// zero: 0 00000000 00000000000000000000000
//  inf: s 11111111 00000000000000000000000
// qnan: _ 11111111 1xxxxxxxxxxxxxxxxxxxxxx
// snan: _ 11111111 0xxxxxxxxxxxxxxxxxxxxxx
//
//     normal: s xxxxxxxx xxxxxxxxxxxxxxxxxxxxxxx
// sub-normal: s 00000000 xxxxxxxxxxxxxxxxxxxxxxx (really small number)
#define F32_EXPONENT_BITS     8
#define F32_EXPONENT_BIAS     0x7F
#define F32_SIGNIFICAND_BITS  23
#define F32_MIN_EXPONENT     (-126)
#define F32_MAX_EXPONENT      127
#define F32_MIN_SUBNORMAL     1.40129846e-45f
#define F32_MIN_NORMAL        1.17549435e-38f
#define F32_MAX_NORMAL        3.40282347e+38f
#define F32_EPSILON           1.19209290e-7f
#define F32_SIGN_MASK         0x80000000
#define F32_EXPONENT_MASK     0x7F800000
#define F32_EXPONENT_SHIFT    23
#define F32_SIGNIFICAND_MASK  0x007FFFFF
#define F32_TWO_POW_25        0x4C000000 
#define F32_TWO_POW_MINUS_25  0x4C000000 
union ieee_binary32 {
    f32 f;
    u32 i;
};

struct separated_float32 {
    f32 mantissa; // preserves sign
    i32 exponent;
};

enum ieee754_number_category {
    IEEE754_Zero,
    IEEE754_Normal,
    IEEE754_Subnormal,
    IEEE754_Infinity,
    IEEE754_NaN
};
#endif // kbmath_h
