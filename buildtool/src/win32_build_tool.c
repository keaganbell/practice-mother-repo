#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "crt_wrapper.h"

#include "types.h"
#include "kbstring.h"
#include "base.h"
#include "lexer.h"
#include "win32_platform.h"
#include "build_tool.h"

void *_arena_push(arena_allocator *arena, u64 size, b32 clear_to_zero);
inline void *win32_virtual_alloc(void *pointer, u64 size, u32 flags);
inline void win32_virtual_free(void *memory);

#include "base.c"
#include "kbstring.c"
#include "lexer.c"
#include "win32_platform.c"
#include "build_tool.c"

int main(void) {
    command cmd = {};
    cmd.source = S8("src\\test.c");
    cmd_cflags_append(&cmd, S8(CFLAGS));
    cmd_cflags_append(&cmd, S8(" /Foobj\\test.obj /Fdpdb\\compiler_test.pdb "));
    cmd_linker_append(&cmd, S8(CLINKER));
    cmd_linker_append(&cmd, S8(" /OUT:bin\\debug\\test.exe /PDB:pdb\\linker_test.pdb" ));
    b32 success = cmd_run_sync(&cmd);
    if (success) {
        // continue building and running everthing else
    }
    else {
        fprintf(stderr, "Failed to build %.*s\n", (int)cmd.source.length, cmd.source.value);
    }
    return 0;
}
