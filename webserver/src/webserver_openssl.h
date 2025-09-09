#pragma once
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/applink.c>

struct openssl {
    SSL_CTX *Context;
};
