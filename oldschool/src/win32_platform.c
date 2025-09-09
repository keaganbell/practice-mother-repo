static void win32_print(String string) {
    DWORD written;
    WriteConsoleA(state.std_output, string.value, string.length, &written, 0);
}
