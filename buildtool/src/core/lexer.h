#ifndef lexer_h
#define lexer_h

typedef enum token_enum {
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
} token_enum;

typedef struct token {
    token_enum type;
    string_c8 text;
} token;

typedef struct tokenizer {
    c8 *at;
    c8 *prev;
} tokenizer;

#endif // lexer_h
