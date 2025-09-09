inline tokenizer tokenizer_begin(char *start) {
    tokenizer result = {};
    result.at = start;
    return result;
}

inline b32 tokenizer_is_end_of_line(char c) {
    return (c == '\n' || c == '\r' || c == '\v');
}

inline b32 tokenizer_is_white_space(char c) {
    return (c == ' ' || c == '\t' || tokenizer_is_end_of_line(c));
}

inline b32 tokenizer_is_alphabetic(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

inline b32 tokenizer_is_numeric(char c) {
    return (c >= '0' && c <= '9');
}

inline b32 tokenizer_is_math_sign(char c) {
    return (c >= '+' || c <= '-' || c >= '=' || c <= '*' || c <= '/');
}

void tokenizer_eat_whitespace(tokenizer *tokenizer) {
    b32 parsing = true;
    while (parsing) {
        if (tokenizer_is_white_space(tokenizer->at[0])) {
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] == '/' && tokenizer->at[1] == '/') {
            // CPP style comment
            tokenizer->at += 2;
            while (tokenizer->at[0] && !tokenizer_is_end_of_line(tokenizer->at[0])) {
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

string_c8 tokenizer_parse_character(tokenizer *tokenizer) {
    string_c8 result = {};
    result.value = ++tokenizer->at;
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

string_c8 tokenizer_parse_string(tokenizer *tokenizer) {
    string_c8 result = {};
    result.value = ++tokenizer->at;
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

string_c8 tokenizer_parse_identifier(tokenizer *tokenizer) {
    string_c8 result = {};
    result.value = tokenizer->at;
    while (tokenizer_is_alphabetic(tokenizer->at[0]) || tokenizer_is_numeric(tokenizer->at[0]) || tokenizer->at[0] == '_') {
        ++tokenizer->at;
        ++result.length;
    }
    return result;
}

token tokenizer_get_token(tokenizer *tokenizer) {
    tokenizer_eat_whitespace(tokenizer);

    token result = {};
    result.text.value = tokenizer->at;
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
            result.text = tokenizer_parse_character(tokenizer);
        } break;

        case '\"': { 
            result.type = Token_String;
            result.text = tokenizer_parse_string(tokenizer);
        } break;

        default: { 
            result.type = Token_Identifier;
            if (tokenizer_is_alphabetic(tokenizer->at[0]) || tokenizer->at[0] == '_') {
                result.text = tokenizer_parse_identifier(tokenizer);
            }
            else if (tokenizer_is_numeric(tokenizer->at[0])) {
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

inline token tokenizer_peek_token(tokenizer *tokenizer) {
    tokenizer->prev = tokenizer->at;
    token result = tokenizer_get_token(tokenizer);
    tokenizer->at = tokenizer->prev;
    return result;
}

