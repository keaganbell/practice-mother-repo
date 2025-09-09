#pragma once

struct entire_file {
    u8 *Contents;
    size_t Filesize;
};

entire_file ReadEntireFile(const char *Filename) {
    entire_file Result = {};
    FILE *File = fopen(Filename, "rb");
    if (File) {
        fseek(File, 0, SEEK_END);
        Result.Filesize = ftell(File);
        LINFO("Reading %s. Filesize is %zu Bytes.", Filename, Result.Filesize);
        rewind(File);
        Result.Contents = (u8 *)malloc(Result.Filesize);
        fread(Result.Contents, 1, Result.Filesize, File);
        fclose(File);
    }
    else {
        LERROR("Failed to open file %s", Filename);
    }
    return Result;
}

void FreeEntireFile(entire_file File) {
    free(File.Contents);
}
