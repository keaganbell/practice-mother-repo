#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>
#include <GL/wglext.h>

#include "defines.h"
#include "log.h"
#include "platform.h"
#include "arena.h"
#include "extendo.h"
#include "windows_opengl.h"
#include "extendo_opengl.h"

static b32 GlobalRunning = true;
static u32 GlobalClientWidth;
static u32 GlobalClientHeight;

#include "extendo_opengl.cpp"
#include "windows_debug.cpp"
#include "windows_opengl.cpp"

#ifndef EXTENDO_DLL_NAME
#define EXTENDO_DLL_NAME "extendo.dll"
#endif

struct program_code {
    b32 IsValid;
    FILETIME LastWriteTime;
    HMODULE CodeDLL;
    program_initialize *Initialize;
    program_update_and_render *UpdateAndRender;
};

static inline FILETIME GetLastWriteTime(char *Filename) {
    FILETIME LastWriteTime = {};
    WIN32_FIND_DATA FindData;
    HANDLE FindHandle = FindFirstFileA(Filename, &FindData);
    if (FindHandle != INVALID_HANDLE_VALUE) {
        LastWriteTime = FindData.ftLastWriteTime;
        FindClose(FindHandle);
    }
    return LastWriteTime;
}

static inline program_code GetProgramCode(char *SourceDLLName) {
    program_code Result = {};
    Result.LastWriteTime = GetLastWriteTime(SourceDLLName);
    CopyFile(SourceDLLName, "temp_" EXTENDO_DLL_NAME, false);
    Result.CodeDLL = LoadLibraryA("temp_" EXTENDO_DLL_NAME);
    if (Result.CodeDLL) {
        Result.Initialize = (program_initialize *)GetProcAddress(Result.CodeDLL, "Initialize");
        Result.UpdateAndRender = (program_update_and_render *)GetProcAddress(Result.CodeDLL, "UpdateAndRender");
        if (Result.Initialize && Result.UpdateAndRender) {
            Result.IsValid = true;
        }
    }
    if (!Result.IsValid) {
        Result.Initialize = InitializeStub;
        Result.UpdateAndRender = UpdateAndRenderStub;
    }
    return Result;
}

static inline void UnloadProgramCode(program_code *Code) {
    Code->IsValid = false;
    Code->Initialize = InitializeStub;
    Code->UpdateAndRender = UpdateAndRenderStub;

    if (Code->CodeDLL) {
        FreeLibrary(Code->CodeDLL);
    }
}

LRESULT CALLBACK MainCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {

        case WM_CREATE: {
            RECT Client;
            GetClientRect(Window, &Client);
            GlobalClientWidth = Client.right - Client.left;
            GlobalClientHeight = Client.bottom - Client.top;
        } break;

        case WM_SIZE: {
            GlobalClientWidth = LOWORD(LParam);
            GlobalClientHeight = HIWORD(LParam);
        } break;

        case WM_CLOSE: {
            GlobalRunning = false;
        } break;

        case WM_DESTROY: {
            GlobalRunning = false;
        } break;

        default:
            return DefWindowProc(Window, Message, WParam, LParam);
    };

    return 0;
}

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CmdLine, int CmdShow) {
    platform_api Platform = {};
    Platform.DebugPrint = PlatformDebugPrint;
    GlobalPlatform = &Platform;

    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = NULL;
    WindowClass.hCursor = NULL;
    WindowClass.lpszClassName = "MainWindowClass";

    RegisterClassA(&WindowClass);

    HWND Window = CreateWindowA(
        WindowClass.lpszClassName,
        "HOTLOAD! BABY!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        Instance,
        NULL
    );

    if (Window) {
        HDC DC = GetDC(Window);
        WindowsSetupOpenGL(DC);

        char *SourceDLLName = EXTENDO_DLL_NAME;
        program_code Program = GetProgramCode(SourceDLLName);

        program_memory Memory = {};
        Memory.PermanentSize = 64*MiB;
        Memory.FrameScratchSize = 1*GiB;
        Memory.PermanentStorage = VirtualAlloc(NULL, Memory.PermanentSize, MEM_COMMIT, PAGE_READWRITE);
        Memory.FrameScratchStorage = VirtualAlloc(NULL, Memory.FrameScratchSize, MEM_COMMIT, PAGE_READWRITE);

        Program.Initialize(&Memory, &Platform);

        while (GlobalRunning) {

            FILETIME FileTime = GetLastWriteTime(SourceDLLName);
            if (CompareFileTime(&Program.LastWriteTime, &FileTime) != 0) {
                UnloadProgramCode(&Program);
                Program = GetProgramCode(SourceDLLName);
            }

            MSG Message = {};
            while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }

            Program.UpdateAndRender(&Memory, &Platform);
            Sleep(17);
        }
    }
    else {
        LFATAL(Platform.DebugPrint, "Failed to create window.");
    }

    return 0;
}
