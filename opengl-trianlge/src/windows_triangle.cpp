#define GL_GLEXT_PROTOTYPES
#include "GL/glcorearb.h"
#include "GL/wglext.h"

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "rendering_math.h"
#include "triangle.h"
#include "windows_opengl.h"
#include "triangle_opengl.h"
#include "input.h"

static b32 GlobalRunning = true;
static u32 GlobalClientWidth;
static u32 GlobalClientHeight;
static b32 GlobalIsActive;
static program_input *NewInput;
static program_input *OldInput;

#include "triangle_opengl.cpp"
#include "windows_opengl.cpp"
#include "triangle.cpp"

LRESULT CALLBACK MainCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch(Message) {

        case WM_CREATE: {
            RECT Client;
            GetClientRect(Window, &Client);
            GlobalClientWidth = Client.right - Client.left;
            GlobalClientHeight = Client.bottom - Client.top;
        } break;

        case WM_ACTIVATE: {
            if (WParam == WA_ACTIVE || WParam == WA_CLICKACTIVE) {
                GlobalIsActive = true;
            }
            else if (WParam == WA_INACTIVE) {
                // TODO: disable input stuff
                GlobalIsActive = false;
            }
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

        case WM_KEYDOWN:
        case WM_KEYUP: {
            b8 WasDown = ((LParam & (1 << 30)) == 0);
            b8 IsUp = ((LParam & (1 << 31)) != 0);
            if (WasDown != IsUp) {
                if (WParam == VK_SPACE) {
                    UpdateButton(BUTTON_KEY_SPACE, NewInput, IsUp);
                }
                else if (WParam == VK_ESCAPE) {
                    UpdateButton(BUTTON_KEY_ESCAPE, NewInput, IsUp);
                }
                else if (WParam == 'W') {
                    UpdateButton(BUTTON_KEY_W, NewInput, IsUp);
                }
                else if (WParam == 'A') {
                    UpdateButton(BUTTON_KEY_A, NewInput, IsUp);
                }
                else if (WParam == 'S') {
                    UpdateButton(BUTTON_KEY_S, NewInput, IsUp);
                }
                else if (WParam == 'D') {
                    UpdateButton(BUTTON_KEY_D, NewInput, IsUp);
                }
                else if (WParam == VK_LEFT) {
                    UpdateButton(BUTTON_KEY_LEFT, NewInput, IsUp);
                }
                else if (WParam == VK_RIGHT) {
                    UpdateButton(BUTTON_KEY_RIGHT, NewInput, IsUp);
                }
                else if (WParam == VK_UP) {
                    UpdateButton(BUTTON_KEY_UP, NewInput, IsUp);
                }
                else if (WParam == VK_DOWN) {
                    UpdateButton(BUTTON_KEY_DOWN, NewInput, IsUp);
                }
            }
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
        "Rendering Practice",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        Instance,
        NULL
    );

    if (Window) {
        HDC DC = GetDC(Window);
        opengl *OpenGL = WindowsSetupOpenGL(DC);

        program_input _InputA = {};
        program_input _InputB = {};
        NewInput = &_InputA;
        OldInput = &_InputB;

        LARGE_INTEGER LastFrameCounter;
        QueryPerformanceCounter(&LastFrameCounter);
        LARGE_INTEGER CounterFrequencyResult;
        QueryPerformanceFrequency(&CounterFrequencyResult);
        i64 CounterFrequency = CounterFrequencyResult.QuadPart;

        while (GlobalRunning) {

            MSG Message = {};
            while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }

            if (GlobalIsActive) {
                b32 ShiftIsUp = GetKeyState(VK_SHIFT) & (1 << 15);
                UpdateButton(BUTTON_KEY_SHIFT, NewInput, !ShiftIsUp);
            }

            LARGE_INTEGER BeginFrameCounter;
            QueryPerformanceCounter(&BeginFrameCounter);
            i64 CounterElapsed = BeginFrameCounter.QuadPart - LastFrameCounter.QuadPart;
            f64 Frametime = (f64)CounterElapsed/CounterFrequency;
            LastFrameCounter = BeginFrameCounter;
            char Title[64] = {};
            sprintf(Title, "Rendering Practice | fps: %.0f", (f32)(1.f/Frametime));
            SetWindowText(Window, Title);

            UpdateAndRender(OpenGL, NewInput);

            program_input *Temp = NewInput;
            NewInput = OldInput;
            *NewInput = {};
            OldInput = Temp;
            NewInput->MousePosition = OldInput->MousePosition;
            for (u32 i = 0; i < BUTTON_TYPE_MAX_COUNT; ++i) {
                NewInput->Buttons[i].Down = OldInput->Buttons[i].Down;
            }

            SwapBuffers(DC);
        }
    }
    else {
        OutputDebugString("Failed to create window.\n");
    }
    return 0;
}

