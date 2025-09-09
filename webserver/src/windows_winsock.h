#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 52422
#define DEFAULT_BUFLEN 4096

struct windows_server {
    // common stuff

    // windows stuff
    WSADATA WSA;
    SOCKET ListenSocket;
    // maybe a pointer to client sockets idk
};

