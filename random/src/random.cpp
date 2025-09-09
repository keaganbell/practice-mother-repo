#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct token {
    char *Text;
    int Length;
};

struct tokenizer {
    char *At;
    int Count;
};

struct file_contents {
    unsigned char *Contents;
    unsigned long FileSize;
};

static inline int IsWhiteSpace(char C) {
    return isspace(C);
}

static inline void EatWhiteSpace(tokenizer *Tokenizer) {
    while (IsWhiteSpace(*Tokenizer->At)) {
        ++Tokenizer->At;
    }
}

static inline file_contents GetFileContents(char *Filename) {
    file_contents Result = {};
    FILE *File = fopen(Filename, "r");
    fseek(File, 0, SEEK_END);
    Result.FileSize = ftell(File);
    rewind(File);
    Result.Contents = (unsigned char *)malloc(Result.FileSize);
    fread(Result.Contents, 1, Result.FileSize, File);
    fclose(File);
    return Result;
}

static inline void FreeFile(file_contents File) {
    if (File.Contents) {
        free(File.Contents);
    }
    else {
        printf("Warning: Tried to free a NULL pointer.\n");
    }
}

static inline token GetNextToken(tokenizer *Tokenizer) {
    token Result = {};
    EatWhiteSpace(Tokenizer);
    Result.Text = Tokenizer->At;
    if (*Tokenizer->At == 0x0d && *Tokenizer->At+1 == 0x0a) {
        printf("End of file reached.\n");
        Result.Text = NULL;
    }
    else {
        while (!IsWhiteSpace(*Tokenizer->At++)) {
            ++Result.Length;
        }
        ++Tokenizer->Count;
    }
    return Result;
}

int main(int Argc, char **Argv) {
    // TODO: handle the output file properly
    for (int i = 1; i < Argc; ++i) {

        file_contents InputFile = GetFileContents(Argv[i]);
        if (InputFile.Contents) {

            tokenizer Tokenizer = {};
            Tokenizer.At = (char *)InputFile.Contents;
            token Next = GetNextToken(&Tokenizer);

            FILE *Output = fopen("random.data", "wb");
            if (Output) {
                while (Next.Text) {
                    char *EndPointer;
                    double Value = strtod(Next.Text, &EndPointer);
                    if (Next.Text != EndPointer) {
                        fwrite(&Value, sizeof(double), 1, Output);
                        char Space[1] = { ' ' };
                        fwrite(Space, 1, 1, Output);
                    }
                    else {
                        printf("Failed to parse %.*s\n", Next.Length, Next.Text);
                    }
                    Next = GetNextToken(&Tokenizer);
                }
            }
            else {
                printf("Failed to open output file.\n");
            }

            FreeFile(InputFile);
        }
        else {
            printf("Failed to open file %s\n", Argv[i]);
        }

    }
    return 0;
}
