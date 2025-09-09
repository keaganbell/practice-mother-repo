#include "oldschool.h"

Win32State state;

#include "win32_platform.c"

void mainCRTStartup() {
    state.std_output = GetStdHandle(STD_OUTPUT_HANDLE);
    String msg = S("Hello, World!\n");
    win32_print(msg);
    ExitProcess(0);
}
