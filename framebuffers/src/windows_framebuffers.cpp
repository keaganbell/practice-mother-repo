//#include "GL/glcorearb.h" // includes windows.h apparently
//#include "GL/wglext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

#include "framebuffers.h"
#include "renderer_opengl.h"

static platform_api _Platform;
static platform_api *Platform;
static u32 GlobalScreenWidth;
static u32 GlobalScreenHeight;

#include "renderer_opengl.cpp"
#include "windows_opengl.cpp"

static b32 GlobalRunning = true;

struct program_code {
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

static inline program_code GetProgramCode() {
    program_code Result = {};
    Result.LastWriteTime = GetLastWriteTime(APP_DLL_NAME);
#ifndef RELEASE_BUILD
    CopyFile(APP_DLL_NAME, "temp_" APP_DLL_NAME, false);
    Result.CodeDLL = LoadLibraryA("temp_" APP_DLL_NAME);
#else
    Result.CodeDLL = LoadLibraryA(APP_DLL_NAME);
#endif
    b32 IsValid = false;
    if (Result.CodeDLL) {
        Result.Initialize = (program_initialize *)GetProcAddress(Result.CodeDLL, "Initialize");
        Result.UpdateAndRender = (program_update_and_render *)GetProcAddress(Result.CodeDLL, "UpdateAndRender");
        if (Result.Initialize && Result.UpdateAndRender) {
            LINFO("Found both functions!");
            IsValid = true;
        }
    }
    if (!IsValid) {
        Result.Initialize = InitializeStub;
        Result.UpdateAndRender = UpdateAndRenderStub;
    }
    return Result;
}

#ifndef RELEASE_BUILD
static inline void UnloadProgramCode(program_code *Code) {
    Code->Initialize = InitializeStub;
    Code->UpdateAndRender = UpdateAndRenderStub;
    if (Code->CodeDLL) {
        FreeLibrary(Code->CodeDLL);
    }
}
#endif

PLATFORM_DEBUG_PRINT(WindowsDebugPrint) {
    char Buffer[2048];
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer, sizeof(Buffer), Message, Args);
    va_end(Args);
    OutputDebugString(Buffer);
}

PLATFORM_VIRTUAL_ALLOCATE(WindowsVirtualAllocate) {
    return VirtualAlloc(NULL, Size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

LRESULT CALLBACK MainCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch (Message) { 
        case WM_CREATE: {
            RECT Client;
            GetClientRect(Window, &Client);
            GlobalScreenWidth = Client.right - Client.left;
            GlobalScreenHeight = Client.bottom - Client.top;
        } break;

        case WM_SIZE: {
            GlobalScreenWidth = LOWORD(LParam);
            GlobalScreenHeight = HIWORD(LParam);
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

static void ProcessMessages(HWND Window) {
    MSG Message = {};
    while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
        if (Message.message == WM_QUIT) {
            GlobalRunning = false;
        }
        switch (Message.message) {

            default: {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }
        }
    }
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, int ShowCommand) {
    Platform = &_Platform;
    Platform->DebugPrint = WindowsDebugPrint;
    Platform->VirtualAllocate = WindowsVirtualAllocate;

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
        "Framebuffers Practice",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        Instance,
        NULL
    );

    if (Window) {
        HDC DC = GetDC(Window);
        opengl *OpenGL = WindowsInitOpenGL(DC);

        program_memory Memory = {};
        Memory.PlatformAPI = Platform;

        program_code Program = GetProgramCode();
        Program.Initialize(&Memory);

        while (GlobalRunning) {
#ifndef RELEASE_BUILD
            FILETIME FileTime = GetLastWriteTime(APP_DLL_NAME);
            if (CompareFileTime(&Program.LastWriteTime, &FileTime) != 0) {
                UnloadProgramCode(&Program);
                Program = GetProgramCode();
            }
#endif
            ProcessMessages(Window);

            render_commands Commands = OpenGLBeginFrame();
            if (Program.UpdateAndRender) {
                Program.UpdateAndRender(&Memory, &Commands);
            }
            OpenGLEndFrame(OpenGL, &Commands);
            SwapBuffers(DC);
        }
    }

    return 0;
}
