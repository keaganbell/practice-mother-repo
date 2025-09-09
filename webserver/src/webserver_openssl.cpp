#pragma once

static void InitializeOpenSSL(openssl *OpenSSL) {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    const SSL_METHOD *Method = TLS_server_method();
    SSL_CTX *CTX = SSL_CTX_new(Method);
    Assert(CTX);

    int ReturnCode = 0;
    ReturnCode = SSL_CTX_use_certificate_file(CTX, "cert/server.crt", SSL_FILETYPE_PEM);
    Assert(ReturnCode > 0);
    ReturnCode = SSL_CTX_use_PrivateKey_file(CTX, "cert/server.key", SSL_FILETYPE_PEM);
    Assert(ReturnCode > 0);

    OpenSSL->Context = CTX;
}
