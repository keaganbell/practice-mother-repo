#ifndef base_h
#define base_h

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

struct Array {
    u64 count;
    u64 capacity;
    void *v;
};

struct EntireFile {
    char *contents;
    u64 size;
};


/*
 * ///////////////////// Strings ////////////////////////////////////
 */
#define STR_EQUAL 0
#define S(x) String { (char *)(x), sizeof((x)) - 1 }
struct String {
    char *string;
    u64 length;
};

struct StrBuilder {
    char *string;
    u64 length;
    u64 cap;
};

/*
 * ///////////////////// Lexer ////////////////////////////////////
 */
enum TokenType {
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
    TokenType type;
    String text;
};

struct Tokenizer {
    char *at;
    char *prev;
};

#endif // base_h
