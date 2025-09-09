#include <stdio.h>

#include "types.h"
#include "base.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "windows_platform.h"

#include "generated/meta.generated.h"

#include "base.cpp"
#include "windows_platform.cpp"

int main(void) { 
    String message = S("Hello, World!\n");
    Print(message);
    getchar();
    return 0;
}
