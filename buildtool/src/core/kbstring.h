#ifndef kbstring_h
#define kbstring_h

#define S8(x)   (string_c8){ .value = (x), .length = sizeof((x)) - 1 }
#define S16(x) (string_c16){ .value = (x), .length = sizeof((x))/2 - 1 }

typedef struct string_c8 {
    c8 *value;
    u64 length;
} string_c8;

typedef struct string_c16 {
    c16 *value;
    u64 length;
} string_c16;

typedef struct string_builder_c8 {
    string_c8 string;
    u64 capacity;
} string_builder_c8;

typedef struct string_builder_c16 {
    string_c16 string;
    u64 capacity;
} string_builder_c16;

#endif // kbstring_h
