#define str_equals(a, b) _Generic((a), \
        string_c16: str_equals_wide, \
        default: str_equals_string)(a, b)

b32 str_equals_string(string_c8 str_a, string_c8 str_b) {
    b32 result = true;
    if (str_a.length != str_b.length) {
        result = false;
    }
    else {
        for (i32 idx = 0; idx < str_a.length; ++idx) {
            if (str_a.value[idx] != str_b.value[idx]) {
                result = false;
                break;
            }
        }
    }
    return result;
}

b32 str_equals_wide(string_c16 str_a, string_c16 str_b) {
    b32 result = true;
    if (str_a.length != str_b.length) {
        result = false;
    }
    else {
        for (i32 idx = 0; idx < str_a.length; ++idx) {
            if (str_a.value[idx] != str_b.value[idx]) {
                result = false;
                break;
            }
        }
    }
    return result;
}

// NOTE: this is for c-style strings. the macro S(x) is for string literals
// e.g., 
//     string_c8 str1 = S("hello");
//
//     const char *msg = "world";
//     string_c8 str2 = str_make(msg);
//
inline string_c8 str_make(c8 *string) {
    string_c8 result = {};
    result.value = (char *)string;
    result.length = str_length(string);
    return result;
}

#define str_slice(str, start, idx) _Generic((str), \
        string_c16: str_slice_wide, \
        default: _str_slice)(str, start, idx)

string_c8 _str_slice(string_c8 str, i32 start, i32 length) {
    string_c8 result = {};
    if (start < 0) start = 0;
    if (start > str.length) start = str.length;
    if (start + length > str.length) length = str.length - start;
    if (length < 0) length = 0;
    result.value = str.value + start;
    result.length = length;
    return result;
}

string_c16 str_slice_wide(string_c16 str, i32 start, i32 length) {
    string_c16 result = {};
    if (start < 0) start = 0;
    if (start > str.length) start = str.length;
    if (start + length > str.length) length = str.length - start;
    if (length < 0) length = 0;
    result.value = str.value + start;
    result.length = length;
    return result;
}

#define str_copy(arena, x) _Generic((x), \
        c16 *: str_copy_wide_cstyle, \
        string_c8:      str_copy_string, \
        default:     str_copy_cstyle)(arena, x)

inline string_c8 str_copy_cstyle(arena_allocator *arena, const char *string) {
    string_c8 result = {};
    u64 length = str_length(string);
    result.value = arena_push_array(arena, c8, length);
    if (result.value) {
        memory_copy(result.value, string, length);
        result.length = length;
    }
    return result;
}

inline string_c16 str_copy_wide_cstyle(arena_allocator *arena, const c16 *string) {
    string_c16 result = {};
    u64 length = str_length(string);
    result.value = (c16 *)arena_push_array(arena, c16, length);
    if (result.value) {
        memory_copy(result.value, string, length*2);
        result.length = length;
    }
    return result;
}

inline string_c8 str_copy_string(arena_allocator *arena, string_c8 string) {
    string_c8 result = {};
    result.value = arena_push_array(arena, c8, string.length);
    if (result.value) {
        memory_copy(result.value, string.value, string.length);
        result.length = string.length;
    }
    return result;
}

#define str_build(arena, builder, x) _Generic((x), \
    string_c8:   strbuilder_append, \
    string_c16:  strbuilder_wide_append, \
    u16 *:    strbuilder_wide_append_cstyle, \
    default:  strbuilder_append_cstyle)(arena, builder, x)

string_c8 strbuilder_append_cstyle(arena_allocator *arena, string_builder_c8 *builder, const char *string) {
    u64 length = str_length(string);
    if (builder->string.length + length > builder->capacity) {
        u64 new_capacity = builder->capacity + length + 1;
        c8 *temp = arena_push_array(arena, c8, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    for (u32 index = 0; index < length; ++index) {
        builder->string.value[builder->string.length++] = string[index];
    }
    return builder->string;
}

string_c8 strbuilder_append(arena_allocator *arena, string_builder_c8 *builder, string_c8 string) {
    if (builder->string.length + string.length > builder->capacity) {
        u64 new_capacity = builder->capacity + string.length + 1;
        c8 *temp = arena_push_array(arena, c8, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    for (u32 index = 0; index < string.length; ++index) {
        builder->string.value[builder->string.length++] = string.value[index];
    }
    return builder->string;
}

string_c16 strbuilder_wide_append_cstyle(arena_allocator *arena, string_builder_c16 *builder, const c16 *string) {
    u64 length = str_length(string);
    if (builder->string.length + length > builder->capacity) {
        u64 new_capacity = builder->capacity + length + 1;
        c16 *temp = arena_push_array(arena, c16, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length*2);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    for (u32 index = 0; index < length; ++index) {
        builder->string.value[builder->string.length++] = string[index];
    }
    return builder->string;
}

string_c16 strbuilder_wide_append(arena_allocator *arena, string_builder_c16 *builder, string_c16 string) {
    if (builder->string.length + string.length > builder->capacity) {
        u64 new_capacity = builder->capacity + string.length + 1;
        c16 *temp = arena_push_array(arena, c16, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length*2);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    for (u32 index = 0; index < string.length; ++index) {
        builder->string.value[builder->string.length++] = string.value[index];
    }
    return builder->string;
}

#define str_null_terminate(arena, builder) _Generic((builder)->string, \
    string_c16:  strbuilder_wide_null_terminate, \
    default: strbuilder_null_terminate)(arena, builder)

string_c8 strbuilder_null_terminate(arena_allocator *arena, string_builder_c8 *builder) {
    if (builder->string.length + 1 > builder->capacity) {
        u64 new_capacity = builder->capacity + 1;
        c8 *temp = arena_push_array(arena, c8, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    builder->string.value[builder->string.length] = '\0';
    return builder->string;
}

string_c16 strbuilder_wide_null_terminate(arena_allocator *arena, string_builder_c16 *builder) {
    if (builder->string.length + 1 > builder->capacity) {
        u64 new_capacity = builder->capacity + 1;
        c16 *temp = arena_push_array(arena, c16, new_capacity);
        kbassert(temp);
        memory_copy(temp, builder->string.value, builder->string.length*2);
        builder->string.value = temp;
        builder->capacity = new_capacity;
    }
    builder->string.value[builder->string.length] = '\0';
    return builder->string;
}
