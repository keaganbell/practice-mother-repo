#pragma once

static void WindowsInitializeServer(windows_server *Server) {
    i32 ReturnCode = -1;

    ReturnCode = WSAStartup(MAKEWORD(2, 2), &Server->WSA);
    Assert(ReturnCode == 0);

    Server->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Assert(Server->ListenSocket != INVALID_SOCKET);

    struct sockaddr_in Address;
    Address.sin_family = AF_INET;
    Address.sin_port = htons(DEFAULT_PORT);
    Address.sin_addr.s_addr = INADDR_ANY;

    ReturnCode = bind(Server->ListenSocket, (struct sockaddr *)&Address, sizeof(Address));
    Assert(ReturnCode != SOCKET_ERROR);

    ReturnCode = listen(Server->ListenSocket, SOMAXCONN);
    Assert(ReturnCode != SOCKET_ERROR);
}
