#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "log.h"
#include "webserver_openssl.h"
#include "windows_winsock.h"

#include "fileio.cpp"
#include "webserver_openssl.cpp"
#include "windows_winsock.cpp"

#ifndef HTML_DIR
#define HTML_DIR "html"
#endif

int main(void) {
    openssl OpenSSL = {};
    InitializeOpenSSL(&OpenSSL);

    entire_file Homepage = ReadEntireFile(HTML_DIR "/index.html");

    windows_server Server = {};
    WindowsInitializeServer(&Server);

    while (1) {
        SOCKET ClientSocket = accept(Server.ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            LERROR("Accept failed\n");
            continue;
        }

        SSL *Ssl = SSL_new(OpenSSL.Context);
        SSL_set_fd(Ssl, (int)ClientSocket);

        if (SSL_accept(Ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            const char *Response = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "Hello, Secure World!";
            SSL_write(Ssl, Response, strlen(Response));
        }

        SSL_shutdown(Ssl);
        SSL_free(Ssl);
        closesocket(ClientSocket);
    }

    return 0;
}
