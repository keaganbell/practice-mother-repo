#ifndef oldschool_h
#define oldschool_h

#include "types.h"
#include "base.h"

// TODO: define this in the build step
#define PLATFORM_WIN32 1

#ifdef PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "win32_platform.h"
#endif

#endif // oldschool_h
