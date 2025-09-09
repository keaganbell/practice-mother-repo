#ifndef base_h
#define base_h

#define function

typedef struct Arena Arena;
typedef struct EntireFile EntireFile;
typedef struct String String;
typedef struct StrBuilder StrBuilder;
typedef enum TokenTypeEnum TokenTypeEnum;
typedef struct Token Token;
typedef struct Tokenizer Tokenizer;

#define Assert(x) if (!(x)) *((u8 *)0) = 1;

/*
 * ///////////////////// Memory ////////////////////////////////////
 */
#define DEFAULT_ARENA_CAPACITY 4096
struct Arena {
    u8 *memory;
    u64 used;
    u64 cap;
    Arena *next;
};

#define ArrayCount(array) (sizeof(array)/sizeof(array[0]))

struct EntireFile {
    char *contents;
    u64 size;
};

/*
 * ///////////////////// Strings ////////////////////////////////////
 */
#define STR_EQUAL 0

struct String {
    char *value;
    u32 length;
};

#define S(x) { .value = (x), .length = sizeof((x)) - 1 }

struct StrBuilder {
    String string;
    u64 cap;
};

/*
 * ///////////////////// Lexer ////////////////////////////////////
 */
enum TokenTypeEnum {
    Token_UnknownToken,

    Token_Colon,
    Token_Semicolon,
    Token_Comma,
    Token_MathSign,
    Token_Asterisk,
    Token_Hashtag,
    Token_Period,
    Token_OpenParen,
    Token_CloseParen,
    Token_OpenCurly,
    Token_CloseCurly,
    Token_OpenSquare,
    Token_CloseSquare,

    Token_Identifier,
    Token_String,
    Token_Character,

    Token_InvalidToken,
    Token_EndOfStream
};

struct Token {
    TokenTypeEnum type;
    String text;
};

struct Tokenizer {
    char *at;
    char *prev;
};

#endif
