#include <math.h>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <GL/glcorearb.h>

#include "defines.h"
#include "log.h"
#include "vector_math.h"
#include "platform.h"
#include "arena.h"
#include "wgl_test.h"
#include "win64_opengl.h"

static b32 GlobalRunning = true;
static u32 GlobalClientWidth;
static u32 GlobalClientHeight;

#include "vector_math.cpp"
#include "shaders.cpp"
#include "win64_opengl.cpp"
#include "wgl_test.cpp"

LRESULT CALLBACK MainCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {

        case WM_CREATE: {
            RECT Client;
            GetClientRect(Window, &Client);
            GlobalClientWidth = Client.right - Client.left;
            GlobalClientHeight = Client.bottom - Client.top;
            LINFO("Width and Height are %u, %u", GlobalClientWidth, GlobalClientHeight);
        } break;

        case WM_SIZE: {
            GlobalClientWidth = LOWORD(LParam);
            GlobalClientHeight = HIWORD(LParam);
            glViewport(0.0, 0.0, GlobalClientWidth, GlobalClientHeight);
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
        "WGL BABY!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        Instance,
        NULL
    );

    HDC DC = GetDC(Window);

    if (Window) {
        SetupOpenGL(DC);
        program_memory Memory = {};
        Memory.PermanentSize = 64*MiB;
        Memory.FrameScratchSize = 1*GiB;
        Memory.PermanentStorage = VirtualAlloc(NULL, Memory.PermanentSize, MEM_COMMIT, PAGE_READWRITE);
        Memory.FrameScratchStorage = VirtualAlloc(NULL, Memory.FrameScratchSize, MEM_COMMIT, PAGE_READWRITE);
        Initialize(&Memory);

        while (GlobalRunning) {
            MSG Message = {};
            while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }
            UpdateAndRender(&Memory);
            SwapBuffers(DC);
            Sleep(17);
        }
    }
    else {
        LFATAL("Failed to create window.");
    }

    return 0;
}
