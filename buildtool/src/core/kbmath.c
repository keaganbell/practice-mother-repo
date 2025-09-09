inline vec2 vec2_add(vec2 a, vec2 b) {
    return (vec2) { a.x + b.x, a.y + b.y };
}

inline vec2 vec2_sub(vec2 a, vec2 b) {
    return (vec2) { a.x - b.x, a.y - b.y };
}

inline vec2 vec2_scale(vec2 a, f32 s) {
    return (vec2) { s*a.x, s*a.y };
}

inline f32 vec2_dot(vec2 a, vec2 b) {
    return a.x*b.x + a.y*b.y;
}

inline vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline vec3 vec3_scale(vec3 a, f32 s) {
    return (vec3) { s*a.x, s*a.y, s*a.z };
}

inline f32 vec3_dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3) {
        a.y*b.z - a.z*b.y,
        a.x*b.z - a.z*b.x,
        a.x*b.y - a.y*b.x
    };
}

inline void mat4_scale(mat4 *out, mat4 *a, mat4 *b) {
    for (u32 j = 0; j < 4; ++j) {
        for (u32 i = 0; i < 4; ++i) {
            f32 sum = 0.0f;
            for (u32 k = 0; k < 4; ++k) {
                sum += a->e[j*4 + k]*b->e[k*4 + i];
            }
            out->e[j*4 + i] = sum;
        }
    }
};

inline void mat4_mult(mat4 *out, mat4 *a, mat4 *b) {
    for (u32 j = 0; j < 4; ++j) {
        for (u32 i = 0; i < 4; ++i) {
            f32 sum = 0.0f;
            for (u32 k = 0; k < 4; ++k) {
                sum += a->e[j*4 + k]*b->e[k*4 + i];
            }
            out->e[j*4 + i] = sum;
        }
    }
};

inline vec4 vec4_transform(mat4 *m, vec4 a) {
    vec4 result = {};
    for (u32 j = 0; j < 4; ++j) {
        f32 sum = 0.0f;
        for (u32 i = 0; i < 4; ++i) {
            sum += m->e[j*4 + i]*a.e[i];
        }
        result.e[j] = sum;
    }
    return result;
};

#if 0
inline b32 math_is_finite(f32 x) {
    b32 result = true;
    IEEEBinary32 val = { x };
    u32 exp_field = val.i & F32_EXPONENT_MASK;
    result = exp_field != F32_EXPONENT_MASK;
    return result;
}

inline b32 math_is_infinite(f32 x) {
    b32 result = true;
    IEEEBinary32 val = { x };
    u32 exp_field = val.i & F32_EXPONENT_MASK;
    u32 sig_field = val.i & F32_SIGNIFICAND_MASK;
    result = (exp_field == F32_EXPONENT_MASK && sig_field == 0);
    return result;
}

inline b32 math_is_nan(f32 x) {
    b32 result = true;
    IEEEBinary32 val = { x };
    u32 exp_field = val.i & F32_EXPONENT_MASK;
    u32 sig_field = val.i & F32_SIGNIFICAND_MASK;
    result = (exp_field == F32_EXPONENT_MASK && sig_field != 0);
    return result;
}

inline b32 math_is_positive(f32 x) {
    b32 result = true;
    IEEEBinary32 val = { x };
    result = (val.i & F32_SIGN_MASK) == 0;
    return result;
}

inline f32 math_fabs(f32 x) {
    IEEEBinary32 val;
    val.f = x;
    val.i = val.i & ~F32_SIGN_MASK;
    return val.f;
}

inline f32 math_set_sign(f32 x, b32 negative) {
    IEEEBinary32 val = { x };
    val.i &= ~F32_SIGN_MASK;
    val.i |= negative ? F32_SIGN_MASK : 0;
    return val.f;
}

inline IEEE754_NumberCategory math_get_f32_category(f32 x) {
    IEEE754_NumberCategory result = IEEE754_Zero;
    IEEEBinary32 val = { x };
    u32 exp_field = val.i & F32_EXPONENT_MASK;
    u32 sig_field = val.i & F32_SIGNIFICAND_MASK;
    if (exp_field == 0) {
        result = (sig_field == 0) ? IEEE754_Zero : IEEE754_Subnormal;
    }
    else if (exp_field == F32_EXPONENT_MASK) {
        result = (sig_field == 0) ? IEEE754_Infinity : IEEE754_NaN;
    }
    else {
        result = IEEE754_Normal;
    }
    return result;
}

SeparatedFloat32 math_separate_f32(f32 x) {
    IEEEBinary32 val = { x };
    f32 mantissa = x;
    i32 exponent = 0;
    switch (math_get_f32_category(x)) {
        case IEEE754_Zero:
        case IEEE754_Infinity:
        case IEEE754_NaN: {
        } break;

        case IEEE754_Normal: {
            IEEEBinary32 val = { x };
            u32 exp_field = val.i & F32_EXPONENT_MASK;
            exponent = (i32)(exp_field >> F32_EXPONENT_SHIFT) - (i32)F32_EXPONENT_BIAS;
            val.i &= ~F32_EXPONENT_MASK;
            val.i |= F32_EXPONENT_BIAS << F32_EXPONENT_SHIFT;
            mantissa = val.f;
        } break;

        case IEEE754_Subnormal: {
            IEEEBinary32 val;
            val.f = x*F32_TWO_POW_25;
            u32 exp_field = val.i & F32_EXPONENT_MASK;
            exponent = (i32)(exp_field >> F32_EXPONENT_SHIFT) - (i32)F32_EXPONENT_BIAS - 25;
            val.i &= ~F32_EXPONENT_MASK;
            val.i |= F32_EXPONENT_BIAS << F32_EXPONENT_SHIFT;
            mantissa = val.f;
        } break;
    }
    return (SeparatedFloat32) { mantissa, exponent };
}
#endif
