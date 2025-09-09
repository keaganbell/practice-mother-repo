inline void cmd_cflags_append(command *cmd, string_c8 str) {
    str_build(&cmd->arena, &cmd->cflags, str);
}

inline void cmd_linker_append(command *cmd, string_c8 str) {
    str_build(&cmd->arena, &cmd->linker, str);
}

b32 cmd_run_sync(command *cmd) {
    b32 result = true;

    string_builder_c8 build = {};
    str_build(&cmd->arena, &build, S8("cl.exe "));
    str_build(&cmd->arena, &build, cmd->cflags.string);
    str_build(&cmd->arena, &build, S8(" "));
    str_build(&cmd->arena, &build, cmd->source);
    str_build(&cmd->arena, &build, S8(" /link "));
    str_build(&cmd->arena, &build, cmd->linker.string);
    str_null_terminate(&cmd->arena, &build);

    STARTUPINFO startup_info = {};
    PROCESS_INFORMATION proc_info = {};
    startup_info.cb = sizeof(startup_info);

    b32 success = CreateProcessA(NULL, build.string.value,
        NULL, NULL, FALSE, 0, NULL, NULL,
        &startup_info, &proc_info
    );
    if (success) {
        // Wait for the process to finish
        WaitForSingleObject(proc_info.hProcess, INFINITE);
        DWORD exit_code = 0;
        GetExitCodeProcess(proc_info.hProcess, &exit_code);
        if (exit_code != 0) {
            fprintf(stderr, "Command returned with error: %d\n", exit_code);
            result = false;
        }
        CloseHandle(proc_info.hProcess);
        CloseHandle(proc_info.hThread);
    }
    else {
        result = false;
        fprintf(stderr, "Failed to start process (error %lu)\n", GetLastError());
    }
    return result;
}
