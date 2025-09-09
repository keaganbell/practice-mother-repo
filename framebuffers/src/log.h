#pragma once

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define Assert(val) { if (!val) { *(u8 *)0 = 1; } }

// This might actually by pretty nice because I can pass any "print" function to this that takes these
// parameters. i.e., I could use another print function for printing text on the screen
#define LTRACE(msg, ...) Platform->DebugPrint("TRACE (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LDEBUG(msg, ...) Platform->DebugPrint("DEBUG (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LINFO(msg, ...) Platform->DebugPrint("INFO  (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LWARN(msg, ...) Platform->DebugPrint("WARN  (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LERROR(msg, ...) Platform->DebugPrint("ERROR (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LFATAL(msg, ...) Platform->DebugPrint("FATAL (%s:%d): %s\n", __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
