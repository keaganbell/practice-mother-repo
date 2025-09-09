#include <windows.h>
#include <GL/gl.h>

#include <stdio.h> // vsnprintf
#include <math.h> // sqrtf, tanf

#include "defines.h"
#include "log.h"
#include "maths.h"
#include "platform.h"
#include "input.h"
#include "compute_opengl.h"
#include "opengl_functions.h"

static u32 GlobalScreenWidth;
static u32 GlobalScreenHeight;
static b32 GlobalRunning = true;
static vec2 GlobalMouseP;

#include "compute_opengl.cpp"
#include "windows_opengl.cpp"

static void *PlatformAllocate(size_t Size) {
    void *Result = VirtualAlloc(0, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!Result) {
        Assert(!"Allocate memory failed");
    }
    return Result;
}

static void PlatformMessageBox(const char *Message, ...) {
    char Buffer[2048] = {};
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer, sizeof(Buffer), Message, Args);
    va_end(Args);
    MessageBox(NULL, Buffer, "Error", MB_ICONERROR | MB_OK);
}

static void PlatformDebugPrint(const char *Message, ...) {
    char Buffer[2048];
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer, sizeof(Buffer), Message, Args);
    va_end(Args);
    OutputDebugString(Buffer);
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
            RECT Client;
            GetClientRect(Window, &Client);
            GlobalScreenWidth = Client.right - Client.left;
            GlobalScreenHeight = Client.bottom - Client.top;
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

static void ProcessMessages(HWND Window, program_input *Input) {
    MSG Message = {};
    while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
        if (Message.message == WM_QUIT) {
            GlobalRunning = false;
        }
        switch (Message.message) {
            case WM_MOUSEMOVE: {
                GlobalMouseP = vec2((f32)LOWORD(Message.lParam), (f32)HIWORD(Message.lParam));
            } break;

            case WM_KEYDOWN:
            case WM_KEYUP: {
                b8 WasDown = ((Message.lParam & (1 << 30)) == 0);
                b8 IsUp = ((Message.lParam & (1 << 31)) != 0);
                if (WasDown != IsUp) {
                    if (Message.wParam == VK_SPACE) {
                        UpdateButton(BUTTON_KEY_SPACE, Input, IsUp);
                    }
                    else if (Message.wParam == VK_ESCAPE) {
                        UpdateButton(BUTTON_KEY_ESCAPE, Input, IsUp);
                    }
                    else if (Message.wParam == 'W') {
                        UpdateButton(BUTTON_KEY_W, Input, IsUp);
                    }
                    else if (Message.wParam == 'A') {
                        UpdateButton(BUTTON_KEY_A, Input, IsUp);
                    }
                    else if (Message.wParam == 'S') {
                        UpdateButton(BUTTON_KEY_S, Input, IsUp);
                    }
                    else if (Message.wParam == 'D') {
                        UpdateButton(BUTTON_KEY_D, Input, IsUp);
                    }
                    else if (Message.wParam == VK_LEFT) {
                        UpdateButton(BUTTON_KEY_LEFT, Input, IsUp);
                    }
                    else if (Message.wParam == VK_RIGHT) {
                        UpdateButton(BUTTON_KEY_RIGHT, Input, IsUp);
                    }
                    else if (Message.wParam == VK_UP) {
                        UpdateButton(BUTTON_KEY_UP, Input, IsUp);
                    }
                    else if (Message.wParam == VK_DOWN) {
                        UpdateButton(BUTTON_KEY_DOWN, Input, IsUp);
                    }
                }
            } break;

            case WM_LBUTTONDOWN: {
                UpdateButton(BUTTON_MOUSE_LEFT, Input, false);
            } break;

            case WM_LBUTTONUP: {
                UpdateButton(BUTTON_MOUSE_LEFT, Input, true);
            } break;

            case WM_RBUTTONDOWN: {
                UpdateButton(BUTTON_MOUSE_RIGHT, Input, false);
            } break;

            case WM_RBUTTONUP: {
                UpdateButton(BUTTON_MOUSE_RIGHT, Input, true);
            } break;


            default: {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }
        }
    }
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PTSTR CommandLine, int CommandShow) {

    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = NULL;
    WindowClass.hCursor = NULL;
    WindowClass.lpszClassName = "Main Window Class";

    RegisterClassA(&WindowClass);

    HWND Window = CreateWindowA(
        WindowClass.lpszClassName,
        "Compute Shader Practice",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, Instance, NULL
    );

    if (Window) {
        HDC DC = GetDC(Window);
        opengl *OpenGL = WindowsInitOpenGL(DC);
        if (OpenGL) {

            program_input _Input = {};
            program_input *Input = &_Input;

            LARGE_INTEGER LastFrameCounter;
            QueryPerformanceCounter(&LastFrameCounter);
            LARGE_INTEGER CounterFrequencyResult;
            QueryPerformanceFrequency(&CounterFrequencyResult);
            i64 CounterFrequency = CounterFrequencyResult.QuadPart;

            while (GlobalRunning) {

                ProcessMessages(Window, Input);

                LARGE_INTEGER BeginFrameCounter;
                QueryPerformanceCounter(&BeginFrameCounter);
                i64 CounterElapsed = BeginFrameCounter.QuadPart - LastFrameCounter.QuadPart;
                f64 Frametime = (f64)CounterElapsed/CounterFrequency;
                LastFrameCounter = BeginFrameCounter;

                char Title[256] = {};
                sprintf(Title, "Compute | fps: %.0f", (f32)(1.f/Frametime));
                SetWindowText(Window, Title);

                //
                // Actual rendering goes here
                //
                EndFrame(OpenGL);

                SwapBuffers(DC);

                program_input TempInput = _Input;
                *Input = {};
                for (u32 i = 0; i < BUTTON_TYPE_MAX_COUNT; ++i) {
                    Input->Buttons[i].Down = TempInput.Buttons[i].Down;
                }
            }
        }
        else {
            LERROR("Failed to Init OpenGL.");
        }
    }
    return 0;
}
