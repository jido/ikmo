#include <stdio.h>
#include <string.h>
#include "b32.h"

int main(int n, char *args[]) {
    char buf[4096];
    char code[b32encoded_size(sizeof(buf))];
    size_t len;
    if (n == 2 && strcmp(args[1], "-c") == 0)
    {
        while ( (len = fread(buf, 1, sizeof(buf), stdin)) )
        {
            size_t size = b32encode(buf, code, len);
            fwrite(code, size, 1, stdout);
        }
    }
    else if (n == 2 && strcmp(args[1], "-d") == 0)
    {
        while ( (len = fread(code, 1, sizeof(code), stdin)) )
        {
            size_t size = b32decode(code, buf, len);
            fwrite(buf, size, 1, stdout);
        }
    }
    else
    {
        puts("Usage: b32enc [ -c | -d ]");
        puts(" encode or decode from stdin to stdout using a base 32 encoding");
    }
}