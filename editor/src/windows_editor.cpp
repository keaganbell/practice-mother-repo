#define GL_GLEXT_PROTOTYPES
#include "GL/glcorearb.h"
#include "GL/wglext.h"

#include "editor.h"

static b32 GlobalRunning = true;
static program_input GlobalInput;

// This is expected to be defined during the build
#ifndef APP_DLL_NAME
#define APP_DLL_NAME "app.dll"
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
#ifndef RELEASE_BUILD
    CopyFile(SourceDLLName, "temp_" APP_DLL_NAME, false);
    Result.CodeDLL = LoadLibraryA("temp_" APP_DLL_NAME);
#else
    Result.CodeDLL = LoadLibraryA(APP_DLL_NAME);
#endif
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

#ifndef RELEASE_BUILD
static inline void UnloadProgramCode(program_code *Code) {
    Code->IsValid = false;
    Code->Initialize = InitializeStub;
    Code->UpdateAndRender = UpdateAndRenderStub;

    if (Code->CodeDLL) {
        FreeLibrary(Code->CodeDLL);
    }
}
#endif

LRESULT CALLBACK MainCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {
        case WM_QUIT:
        case WM_CLOSE:
        case WM_DESTROY: {
            GlobalRunning = false;
        } break;

        default:
            return DefWindowProc(Window, Message, WParam, LParam);
    };
    return 0;
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
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
        "Editor",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        Instance,
        NULL
    );

    if (Window) {
        HDC DC = GetDC(Window);
        // TODO: Stand up opengl
        char *SourceDLLName = APP_DLL_NAME;
        program_code Program = GetProgramCode(SourceDLLName);

        program_memory Memory = {};

        if (Program.Initialize) {
            Program.Initialize(&Memory);
        }

        while (GlobalRunning) {
            MSG Message = {};
            while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }

            if (Program.UpdateAndRender) {
                Program.UpdateAndRender(&Memory, &GlobalInput);
            }
        }
    }
    return 0;
}
