#ifndef windows_platform_h
#define windows_platform_h

typedef struct Win32State Win32State;

struct Win32State {
    HANDLE std_output;
};

#endif // windows_platform_h
