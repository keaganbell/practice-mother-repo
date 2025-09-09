#ifndef base_cpp
#define base_cpp

/*
 * ///////////////////   Arenas   ///////////////////////////////
 */
function static inline void CreateArena(Arena *arena, u64 cap) {
    *arena = {};
    arena->memory = (u8 *)PlatformAllocate(cap);
    arena->cap = cap;
}

// NOTE: I always leave room at the end of the buffer to push on an arena struct so that they
// can be chained together. There are other alternative implementations
#define PushStruct(arena, type) (type *)_PushArena(arena, sizeof(type), true)
#define PushArray(arena, type, count) (type *)_PushArena(arena, sizeof(type)*count, true)
#define PushString(arena, string) (char *)_PushArena(arena, sizeof(u8)*string.length, true);
#define PushSize(arena, size) (u8 *)_PushArena(arena, size, true);
function static void *_PushArena(Arena *arena, u64 size, b32 clearToZero) {
    void *result = NULL;
    u64 newUsed = arena->used + size;
    if (newUsed < arena->cap + sizeof(Arena)) {
        result = arena->memory + arena->used;
        arena->used = newUsed;
    }
    else {
        if (arena->next == NULL) {
            arena->next = PushStruct(arena, Arena);
            Assert(arena->next);
            if (size < DEFAULT_ARENA_CAPACITY) {
                CreateArena(arena->next, DEFAULT_ARENA_CAPACITY);
            }
            else {
                CreateArena(arena->next, size + sizeof(Arena));
            }
        }
        result = _PushArena(arena->next, size, clearToZero);
    }
    if (result && clearToZero) {
        memset(result, 0, size);
    }
    return result;
}

/*
 * ///////////////////   Arrays   ///////////////////////////////
 */
function static void *ArrayGrow(Arena *arena, ) {
}

/*
 * ///////////////////    Strings  ///////////////////////////////
 */
function static i32 StrEquals(String strA, String strB) {
    i32 result = 0;
    for (u32 index = 0; index < strA.length; ++index) {
        if (index < strB.length) {
            if (strA.string[index] != strB.string[index]) {
                result = strB.length - index;
                break;
            }
        }
        else {
            result = index - strB.length;
            break;
        }
    }
    return result;
}

function static inline u64 StrLength(const char *string) {
    u64 count = 0;
    while (*string) {
        ++string;
        ++count;
    }
    return count;
}

// NOTE: this is for c-style strings. the macro S(x) is for string literals
// e.g., 
//     String str1 = S("hello");
//
//     const char *msg = "world";
//     String str2 = MakeString(msg);
//
function static inline String MakeString(const char *string) {
    String result = {};
    result.string = (char *)string;
    result.length = StrLength(string);
    return result;
}

function static String SplitString(String str, i32 index) {
    String result = {};
    if (index >= 0) {
        if (index < str.length) {
            result.string = str.string + index;
            result.length = index + 1;
        }
        else {
            result = str;
        }
    }
    else {
        if (str.length + index > 0) {
            result.string = str.string + str.length + index;
            result.length = -1*index;
        }
        else {
            result = str;
        }
    }
    return result;
}

function static inline String StrCopy(Arena *arena, const char *string) {
    String result = {};
    u64 length = StrLength(string);
    result.string = PushArray(arena, char, length);
    memcpy(result.string, string, length);
    result.length = length;
    return result;
}

function static inline String StrCopy(Arena *arena, String string) {
    String result = {};
    result.string = PushString(arena, string);
    memcpy(result.string, string.string, string.length);
    result.length = string.length;
    return result;
}

function static inline StrBuilder CreateStrBuilder(Arena *arena, u64 size) {
    StrBuilder result = {};
    result.string = (char *)PushSize(arena, size);
    result.cap = size;
    return result;
}

function static String StrBuild(Arena *arena, StrBuilder *builder, const char *string) {
    u64 strLength = StrLength(string);
    if (builder->length + strLength > builder->cap) {
        u64 newCap = builder->cap + strLength;
        char *temp = PushArray(arena, char, newCap);
        memcpy(temp, builder->string, builder->length);
        builder->string = temp;
        builder->cap = newCap;
    }
    for (u32 index = 0; index < strLength; ++index) {
        builder->string[builder->length++] = string[index];
    }
    return { builder->string, builder->length };
}

function static String StrBuild(Arena *arena, StrBuilder *builder, String string) {
    if (builder->length + string.length > builder->cap) {
        u64 newCap = builder->cap + string.length;
        char *temp = PushArray(arena, char, newCap);
        memcpy(temp, builder->string, builder->length);
        builder->string = temp;
        builder->cap = newCap;
    }
    for (u32 index = 0; index < string.length; ++index) {
        builder->string[builder->length++] = string.string[index];
    }
    return { builder->string, builder->length };
}

function static String StrBuildTerminate(Arena *arena, StrBuilder *builder) {
    if (builder->length + 1 > builder->cap) {
        u64 newCap = builder->cap + 1;
        char *temp = PushArray(arena, char, newCap);
        memcpy(temp, builder->string, builder->length);
        builder->string = temp;
        builder->cap = newCap;
    }
    builder->string[builder->length++] = '\0';
    return { builder->string, builder->length };
}

function static inline b32 IsValid(String string) {
    return (string.string != NULL);
}

function static void Print(String str, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, sizeof(buffer), str.string, args);
    printf("%s", buffer);
    va_end(args);
}

/*
 * ///////////////////   Files   ///////////////////////////////
 */
function static inline b32 IsValid(EntireFile file) {
    return (file.contents && file.size > 0);
}

function static inline b32 IsEndOfLine(char c) {
    return (c == '\n' || c == '\r');
}

function static inline b32 IsWhiteSpace(char c) {
    return (c == ' ' || c == '\t' || IsEndOfLine(c));
}

function static inline b32 IsAlphabetic(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

function static inline b32 IsMathSign(char c) {
    return (c >= '+' || c <= '-' || c >= '=' || c <= '*' || c <= '/');
}

// TODO: add more symbols like f, 0L etc (MiB, GiB?), leading minus sign?
function static inline b32 IsNumeric(char c) {
    return (c >= '0' && c <= '9');
}

function static inline String ParseCharacter(Tokenizer *tokenizer) {
    String result = {};
    result.string = ++tokenizer->at;
    while (tokenizer->at[0] != '\'') {
        if (tokenizer->at[0] == '\\') {
            ++tokenizer->at;
            ++result.length;
        }
        ++tokenizer->at;
        ++result.length;
    }
    ++tokenizer->at;
    return result;
}

function static String ParseString(Tokenizer *tokenizer) {
    String result = {};
    result.string = ++tokenizer->at;
    while (tokenizer->at[0] != '\"') {
        if (tokenizer->at[0] == '\\') {
            ++tokenizer->at;
            ++result.length;
        }
        ++tokenizer->at;
        ++result.length;
    }
    ++tokenizer->at;
    return result;
}

function static String ParseIdentifier(Tokenizer *tokenizer) {
    String result = {};
    result.string = tokenizer->at;
    while (IsAlphabetic(tokenizer->at[0]) || IsNumeric(tokenizer->at[0]) || tokenizer->at[0] == '_') {
        ++tokenizer->at;
        ++result.length;
    }
    return result;
}

function static void SkipWhiteSpace(Tokenizer *tokenizer) {
    b32 parsing = true;
    while (parsing) {
        if (IsWhiteSpace(tokenizer->at[0])) {
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] == '/' && tokenizer->at[1] == '/') {
            // CPP style comment
            tokenizer->at += 2;
            while (tokenizer->at[0] && !IsEndOfLine(tokenizer->at[0])) {
                ++tokenizer->at;
            }
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] == '/' && tokenizer->at[1] == '*') {
            // C style comment
            tokenizer->at += 2;
            while (tokenizer->at[0] && !(tokenizer->at[0] == '*' && (tokenizer->at[1] && tokenizer->at[1] == '/'))) {
                ++tokenizer->at;
            }
            tokenizer->at += 2;
        }
        else {
            parsing = false;
        }
    }
}

function static Token GetToken(Tokenizer *tokenizer) {
    SkipWhiteSpace(tokenizer);

    Token result = {};
    result.text.string = tokenizer->at;
    result.text.length = 1;
    switch (tokenizer->at[0]) {
        case  ':':  { result.type = Token_Colon;       ++tokenizer->at; } break;
        case  ';':  { result.type = Token_Semicolon;   ++tokenizer->at; } break;
        case  ',':  { result.type = Token_Comma;       ++tokenizer->at; } break;
        case  '*':  { result.type = Token_Asterisk;    ++tokenizer->at; } break;
        case  '#':  { result.type = Token_Hashtag;     ++tokenizer->at; } break;
        case  '.':  { result.type = Token_Period;      ++tokenizer->at; } break;
        case  '(':  { result.type = Token_OpenParen;   ++tokenizer->at; } break;
        case  ')':  { result.type = Token_CloseParen;  ++tokenizer->at; } break;
        case  '{':  { result.type = Token_OpenCurly;   ++tokenizer->at; } break;
        case  '}':  { result.type = Token_CloseCurly;  ++tokenizer->at; } break;
        case  '[':  { result.type = Token_OpenSquare;  ++tokenizer->at; } break;
        case  ']':  { result.type = Token_CloseSquare; ++tokenizer->at; } break;
        case  '\0': { result.type = Token_EndOfStream; } break;

        case '\'': { 
            result.type = Token_Character;
            result.text = ParseCharacter(tokenizer);
        } break;

        case '\"': { 
            result.type = Token_String;
            result.text = ParseString(tokenizer);
        } break;

        default: { 
            result.type = Token_Identifier;
            if (IsAlphabetic(tokenizer->at[0]) || tokenizer->at[0] == '_') {
                result.text = ParseIdentifier(tokenizer);
            }
            else if (IsNumeric(tokenizer->at[0])) {
                ++tokenizer->at;
            }
            else {
                result.type = Token_UnknownToken;
                ++tokenizer->at;
            }
        } break;
    }

    return result;
}

function static Token PeekToken(Tokenizer *tokenizer) {
    tokenizer->prev = tokenizer->at;
    Token result = GetToken(tokenizer);
    tokenizer->at = tokenizer->prev;
    return result;
}

function static inline Tokenizer BeginTokenizer(char *Text) {
    Tokenizer result = {};
    result.at = Text;
    return result;
}

#endif // base_cpp
