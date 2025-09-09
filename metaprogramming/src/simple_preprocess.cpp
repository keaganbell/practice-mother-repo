#include <stdio.h> // printf
#include <stdarg.h> // va_list, va_start

#include "types.h"
#include "base.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "windows_platform.h"

#include "base.cpp"
#include "windows_platform.cpp"

static String ParseFunction(Arena *arena, Tokenizer *tokenizer, Token token) {
    StrBuilder builder = CreateStrBuilder(arena, 512);
    for (; token.type != Token_CloseParen; token = GetToken(tokenizer)) {
        if (token.type == Token_EndOfStream) {
            break;
        }
        StrBuild(arena, &builder, token.text);
        Token next = PeekToken(tokenizer);
        if (token.type != Token_Asterisk &&
                token.type != Token_OpenParen &&
                token.type != Token_Period &&
                next.type  != Token_OpenParen &&
                next.type  != Token_CloseParen &&
                next.type  != Token_Comma) {
            StrBuild(arena, &builder, " ");
        }
    }
    StrBuild(arena, &builder, ");\n");
    return { builder.string, builder.length };
}

int main(int argc, char **argv) {

    Arena _arena = {};
    Arena *arena = &_arena;
    CreateArena(arena, 2*MiB);

    if (argc == 1) {
    }
    else {
        String srcDir = {};
        String output = {};
        String *excludedFiles = NULL;
        u32 excludedFileCount = 0;

        StrBuilder argsBuilder = CreateStrBuilder(arena, 1024);
        for (i32 idx = 1; idx < argc; ++idx) {
            String arg = MakeString(argv[idx]);
            if (StrEquals(arg, S("-d")) == STR_EQUAL) {
                if (idx < argc) {
                    srcDir = MakeString(argv[++idx]);
                }
            }
            else if (StrEquals(arg, S("-o")) == STR_EQUAL) {
                if (idx < argc) {
                    output = MakeString(argv[++idx]);
                }
            }
            else if (StrEquals(arg, S("-ed")) == STR_EQUAL) {
            }
            else if (StrEquals(arg, S("-e")) == STR_EQUAL) {
            }
        }

        Array test = MakeArray(Strings, 32);

        String projdir = StrCopy(arena, PROJDIR); 

        String filenames[] = {
            S("base.cpp"),
            S("windows_platform.cpp")
        };

        printf("#ifndef meta_generated_h\n");
        printf("#define meta_generated_h\n");

        u32 fileCount = ArrayCount(filenames);
        for (i32 index = 0; index < fileCount; ++index) {
            String filename = filenames[index];
            StrBuilder builder = CreateStrBuilder(arena, 256);
            String filepath = StrBuild(arena, &builder, projdir);
            filepath = StrBuild(arena, &builder, "src/");
            filepath = StrBuild(arena, &builder, filename);
            EntireFile sourceFile = PlatformReadFile(arena, filepath.string);

            printf("\n// %.*s\n", (int)filename.length, filename.string);

            if (IsValid(sourceFile)) {
                Tokenizer _tokenizer = BeginTokenizer(sourceFile.contents);
                Tokenizer *tokenizer = &_tokenizer;
                b32 parsing = true;
                while (parsing) {
                    Token token = GetToken(tokenizer);
                    if (StrEquals(token.text, S("function")) == STR_EQUAL) {
                        String declaration = ParseFunction(arena, tokenizer, token);
                        Print(declaration);
                    }
                    if (token.type == Token_EndOfStream) {
                        parsing = false;
                    }
                }
            }
            else {
                printf("// Source file %.*s was invalid.\n", (int)filename.length, filename.string);
            }
        }

        printf("\n#endif // meta_generated_h\n");
    }
    return 0;
}
